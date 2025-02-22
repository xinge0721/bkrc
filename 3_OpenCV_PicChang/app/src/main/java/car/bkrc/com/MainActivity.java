package car.bkrc.com;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.List;

import car.bkrc.com.utils.form;

public class MainActivity extends AppCompatActivity {

    private static final int PICK_IMAGE_REQUEST = 1;
    private ImageView imageView;
    private TextView resultTextView;
    private form formDetector;
    private Bitmap currentBitmap;

    // 参数默认值
    private int binaryThreshold = 150;
    private double houghParam1 = 100;
    private double houghParam2 = 175;
    private double minContourArea = 1000;
    private int houghMinRadius = 0;
    private int houghMaxRadius = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        formDetector = new form();
        imageView = findViewById(R.id.imageView);
        resultTextView = findViewById(R.id.resultTextView);

        setupSeekBars();
        setupButtons();
    }
    private Button btnClear;

    private void setupSeekBars() {
        SeekBar sbThreshold = findViewById(R.id.sbThreshold);
        SeekBar sbParam1 = findViewById(R.id.sbParam1);
        SeekBar sbParam2 = findViewById(R.id.sbParam2);
        SeekBar sbMinContourArea = findViewById(R.id.sbMinContourArea);
        SeekBar sbMinRadius = findViewById(R.id.sbMinRadius);
        SeekBar sbMaxRadius = findViewById(R.id.sbMaxRadius);

        btnClear = findViewById(R.id.btnClear);
        btnClear.setOnClickListener(v -> {
            resultTextView.setText(""); // 清空识别结果
            // 可选：同时清除图片显示
            // imageView.setImageBitmap(null);
            // currentBitmap = null;
        });
        // 初始化滑块位置
        sbThreshold.setProgress(binaryThreshold);
        sbParam1.setProgress((int) houghParam1);
        sbParam2.setProgress((int) houghParam2);
        sbMinContourArea.setProgress((int) minContourArea);
        sbMinRadius.setProgress(houghMinRadius);
        sbMaxRadius.setProgress(houghMaxRadius);

        // 阈值滑块监听
        sbThreshold.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                formDetector.setBinaryThreshold(progress,R.id.imageView);
            }
            @Override public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        // 霍夫参数1监听
        sbParam1.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                houghParam1 = progress;
                formDetector.setHoughParams(houghParam1, houghParam2);
            }
            @Override public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        // 霍夫参数2监听
        sbParam2.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                houghParam2 = progress;
                formDetector.setHoughParams(houghParam1, houghParam2);
            }
            @Override public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        // 最小轮廓面积滑块监听
        sbMinContourArea.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                formDetector.setMinContourArea(progress);
            }
            @Override public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        // 霍夫最小半径滑块监听
        sbMinRadius.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                formDetector.setHoughMinRadius(progress);
            }
            @Override public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        // 霍夫最大半径滑块监听
        sbMaxRadius.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                houghMaxRadius = progress;
                formDetector.setHoughParams(houghParam1, houghParam2, houghMinRadius, houghMaxRadius);
            }
            @Override public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override public void onStopTrackingTouch(SeekBar seekBar) {}
        });
    }

    private void setupButtons() {
        // 选择图片按钮
        Button btnSelect = findViewById(R.id.selectImageButton);
        btnSelect.setOnClickListener(v -> openImagePicker());

        // 识别按钮
        Button btnDetect = findViewById(R.id.btnDetect);
        btnDetect.setOnClickListener(v -> {
            if (currentBitmap != null) {
                detectAndDisplayResults(currentBitmap);
            } else {
                Toast.makeText(this, "请先选择图片", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void openImagePicker() {
        Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        startActivityForResult(intent, PICK_IMAGE_REQUEST);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == PICK_IMAGE_REQUEST && resultCode == RESULT_OK && data != null) {
            Uri imageUri = data.getData();
            try {
                currentBitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(), imageUri);
                imageView.setImageBitmap(currentBitmap);
            } catch (IOException e) {
                e.printStackTrace();
                Toast.makeText(this, "图片加载失败", Toast.LENGTH_SHORT).show();
            }
        }
    }

    private void detectAndDisplayResults(Bitmap bitmap) {
        new Thread(() -> {
            final List<String> results = formDetector.detectMultiple(bitmap);
            runOnUiThread(() -> resultTextView.setText("识别结果：" + results.toString()));
        }).start();
    }
}