package car.bkrc.com.utils;

import android.graphics.Bitmap;
import android.util.Log;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.Point;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

import java.util.ArrayList;
import java.util.List;

public class ShapeRecognizer {
    private static final String TAG = "ShapeRecognizer";
    private final OpencvUtils cvUtils = new OpencvUtils();

    public AnalysisResult processImage(Bitmap bitmap) {
        Mat srcMat = cvUtils.bitmapToMat(bitmap);
        Mat processedMat = preprocessImage(srcMat);
        List<ColorShapeInfo> results = analyzeImage(processedMat, srcMat);
        return new AnalysisResult(results, processedMat);
    }

    private Mat preprocessImage(Mat srcMat) {
        Mat blurred = new Mat();
        Imgproc.GaussianBlur(srcMat, blurred, new Size(5, 5), 0);
        Imgproc.cvtColor(blurred, blurred, Imgproc.COLOR_BGR2GRAY);
        Imgproc.threshold(blurred, blurred, 130, 255, Imgproc.THRESH_BINARY);

        Mat processed = new Mat();
        Imgproc.cvtColor(srcMat, processed, Imgproc.COLOR_BGR2GRAY);
        Imgproc.threshold(processed, processed, 130, 255, Imgproc.THRESH_BINARY);
        Imgproc.dilate(processed, processed, Mat.ones(3, 3, CvType.CV_8U));

        return processed;
    }

    private List<ColorShapeInfo> analyzeImage(Mat binaryMat, Mat srcMat) {
        List<ColorShapeInfo> results = new ArrayList<>();
        List<MatOfPoint> contours = cvUtils.findContoursList(binaryMat);

        for (MatOfPoint contour : contours) {
            Point[] points = cvUtils.checkPoint(contour.toArray());
            String color = cvUtils.findColor(srcMat, points);
            String shape = cvUtils.findShape(points);

            if (!"ColorError".equals(color) && !"ShapeError".equals(shape)) {
                results.add(new ColorShapeInfo(color, shape));
            }
        }
        return results;
    }

    public static class AnalysisResult {
        public final List<ColorShapeInfo> results;
        public final Mat processedMat;

        public AnalysisResult(List<ColorShapeInfo> results, Mat processedMat) {
            this.results = results;
            this.processedMat = processedMat;
        }
    }
}