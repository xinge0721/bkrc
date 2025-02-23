package car.bkrc.com;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import car.bkrc.com.utils.OpencvUtils;
import car.bkrc.com.utils.ShapeRecognizer;
import car.bkrc.com.utils.ColorShapeInfo;

public class MainActivity extends AppCompatActivity {
    private static final int PICK_IMAGE_REQUEST = 1;

    // 界面组件
    private ImageView imageViewOriginal;
    private ImageView imageViewBinary;
    private ImageView imageViewContours;
    private TextView resultTextView;

    // 核心组件
    private Bitmap currentBitmap;
    private Timer analysisTimer;
    private final ShapeRecognizer shapeRecognizer = new ShapeRecognizer();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initViews();
        setupButtons();
    }

    private void initViews() {
        imageViewOriginal = findViewById(R.id.imageViewOriginal);
        imageViewBinary = findViewById(R.id.imageViewBinary);
        imageViewContours = findViewById(R.id.imageViewContours);
        resultTextView = findViewById(R.id.resultTextView);
    }

    private void setupButtons() {
        // 图片选择按钮
        findViewById(R.id.btnSelect).setOnClickListener(v -> openImagePicker());

        // 开始分析按钮
        findViewById(R.id.btnAnalyze).setOnClickListener(v -> {
            if (currentBitmap != null) {
                startAnalysis();
            } else {
                showToast("请先选择图片");
            }
        });
    }

    private void startAnalysis() {
        cancelPreviousAnalysis();
        analysisTimer = new Timer();
        new Thread(() -> {
            analysisTimer.schedule(new AnalysisTask(currentBitmap), 0, 200);
            runOnUiThread(() -> resultTextView.setText("分析中..."));
        }).start();
    }

    private class AnalysisTask extends TimerTask {
        private final Bitmap sourceBitmap;
        private int threshold = 130;
        private int retryCount = 0;

        public AnalysisTask(Bitmap bitmap) {
            this.sourceBitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);
        }

        @Override
        public void run() {
            try {
                // 使用 ShapeRecognizer 处理图像
                ShapeRecognizer.AnalysisResult result = shapeRecognizer.processImage(sourceBitmap);
                Mat srcMat = new Mat();
                Utils.bitmapToMat(sourceBitmap, srcMat);

                // 绘制轮廓
                Mat contourMat = drawContoursAndPoints(result.processedMat, srcMat);
                updateImageViews(srcMat, result.processedMat, contourMat);

                if (!result.results.isEmpty()) {
                    handleAnalysisSuccess(result.results);
                } else if (shouldRetry()) {
                    adjustThreshold();
                } else {
                    handleAnalysisFailure();
                }

                // 释放 Mat 资源
                srcMat.release();
                contourMat.release();
            } catch (Exception e) {
                handleError(e);
            }
        }

        private Mat drawContoursAndPoints(Mat binaryMat, Mat srcMat) {
            Mat contourMat = srcMat.clone();
            List<MatOfPoint> contours = new ArrayList<>();
            Mat hierarchy = new Mat();

            Imgproc.findContours(binaryMat, contours, hierarchy,
                    Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

            for (MatOfPoint contour : contours) {
                Imgproc.drawContours(contourMat, contours, contours.indexOf(contour),
                        new Scalar(0, 255, 0), 2);

                Point[] points = new OpencvUtils().checkPoint(contour.toArray());
                for (Point p : points) {
                    Imgproc.circle(contourMat, p, 5, new Scalar(255, 0, 0), -1);
                }
            }
            return contourMat;
        }

        private void updateImageViews(Mat srcMat, Mat binaryMat, Mat contourMat) {
            runOnUiThread(() -> {
                imageViewOriginal.setImageBitmap(matToBitmap(srcMat));
                imageViewBinary.setImageBitmap(matToBitmap(binaryMat));
                imageViewContours.setImageBitmap(matToBitmap(contourMat));
            });
        }

        private boolean shouldRetry() {
            return ++retryCount <= 5;
        }

        private void adjustThreshold() {
            if (threshold <= 180) {
                threshold += 10;
            } else {
                threshold = 130;
                retryCount = 6;
            }
        }
    }

    // ======== 辅助方法 ========
    private Bitmap matToBitmap(Mat mat) {
        if (mat == null || mat.empty() || mat.cols() <= 0 || mat.rows() <= 0) {
            Log.e("MainActivity", "Invalid Mat");
            return null;
        }
        Bitmap bitmap = Bitmap.createBitmap(mat.cols(), mat.rows(), Bitmap.Config.ARGB_8888);
        Utils.matToBitmap(mat, bitmap);
        return bitmap;
    }

    private void handleAnalysisSuccess(List<ColorShapeInfo> results) {
        if (analysisTimer != null) {
            analysisTimer.cancel();
            analysisTimer.purge();
            analysisTimer = null;
        }
        runOnUiThread(() -> {
            StringBuilder sb = new StringBuilder("识别结果：\n");
            for (int i = 0; i < results.size(); i++) {
                sb.append(i + 1).append(". ")
                        .append(results.get(i)).append("\n");
            }
            resultTextView.setText(sb.toString());
        });
    }

    private void handleAnalysisFailure() {
        if (analysisTimer != null) {
            analysisTimer.cancel();
            analysisTimer.purge();
            analysisTimer = null;
        }
        runOnUiThread(() -> {
            resultTextView.setText("识别失败，请尝试其他图片");
            showToast("无法识别目标特征");
        });
    }

    private void cancelPreviousAnalysis() {
        if (analysisTimer != null) {
            analysisTimer.cancel();
            analysisTimer.purge();
        }
    }

    private void handleError(Exception e) {
        Log.e("CV_Error", e.getMessage());
        runOnUiThread(() -> showToast("处理出错：" + e.getMessage()));
        cancelPreviousAnalysis();
    }

    // ======== 图片选择相关 ========
    private void openImagePicker() {
        Intent intent = new Intent(Intent.ACTION_PICK);
        intent.setType("image/*");
        startActivityForResult(intent, PICK_IMAGE_REQUEST);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == PICK_IMAGE_REQUEST && resultCode == RESULT_OK && data != null) {
            try {
                Uri imageUri = data.getData();
                currentBitmap = MediaStore.Images.Media.getBitmap(getContentResolver(), imageUri);
                imageViewOriginal.setImageBitmap(currentBitmap);
            } catch (IOException e) {
                showToast("图片加载失败");
            }
        }
    }

    private void showToast(String message) {
        runOnUiThread(() -> Toast.makeText(this, message, Toast.LENGTH_SHORT).show());
    }
}