package car.bkrc.com.utils;



import android.graphics.Bitmap;
import android.util.Log;

import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfInt;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;
import org.opencv.imgproc.Moments;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


class ShapeColorDetector {

    // HSV颜色范围定义 (根据你的实际场景调整阈值)
    private static final Scalar[] RED_LOW = {new Scalar(0, 100, 100), new Scalar(160, 100, 100)};
    private static final Scalar[] RED_HIGH = {new Scalar(10, 255, 255), new Scalar(180, 255, 255)};
    private static final Scalar BLUE_LOW = new Scalar(90, 100, 100);
    private static final Scalar BLUE_HIGH = new Scalar(130, 255, 255);
    private static final Scalar GREEN_LOW = new Scalar(35, 100, 100);
    private static final Scalar GREEN_HIGH = new Scalar(85, 255, 255);
    private static final Scalar YELLOW_LOW = new Scalar(20, 100, 100);
    private static final Scalar YELLOW_HIGH = new Scalar(35, 255, 255);

    /**
     * 在指定区域内识别颜色
     * @param srcImage    原始图像 (BGR格式)
     * @param roiRegion   AdvancedShapeDetector提供的形状区域坐标（矩形框）
     * @return 颜色名称 (如 "red", "blue"), 若无法识别返回 "unknown"
     */
    public String detectColorInRegion(Mat srcImage, Rect roiRegion) {
        // 1. 裁剪目标区域
        Mat roi = new Mat(srcImage, roiRegion);

        // 2. 转HSV颜色空间
        Mat hsvMat = new Mat();
        Imgproc.cvtColor(roi, hsvMat, Imgproc.COLOR_BGR2HSV);

        // 3. 计算颜色直方图（取核心区域减少边缘干扰）
        int centerX = hsvMat.cols() / 2;
        int centerY = hsvMat.rows() / 2;
        int sampleSize = Math.min(20, Math.min(roiRegion.width, roiRegion.height)); // 取中心20x20区域
        Rect centerRect = new Rect(centerX-10, centerY-10, sampleSize, sampleSize);
        Mat centerHsv = new Mat(hsvMat, centerRect);

        // 4. 计算平均HSV值
        Scalar avgHsv = Core.mean(centerHsv);
        double hue = avgHsv.val[0];
        double sat = avgHsv.val[1];
        double val = avgHsv.val[2];

        // 5. 判断颜色
        if (isInRange(hue, sat, val, RED_LOW, RED_HIGH)) {
            return "red";
        } else if (isInRange(hue, sat, val, BLUE_LOW, BLUE_HIGH)) {
            return "blue";
        } else if (isInRange(hue, sat, val, GREEN_LOW, GREEN_HIGH)) {
            return "green";
        } else if (isInRange(hue, sat, val, YELLOW_LOW, YELLOW_HIGH)) {
            return "yellow";
        }
        return "unknown";
    }

    /**
     * 判断HSV值是否在范围内（支持多区间，如红色）
     */
    private boolean isInRange(double h, double s, double v, Scalar[] lows, Scalar[] highs) {
        for (int i=0; i<lows.length; i++) {
            if (h >= lows[i].val[0] && h <= highs[i].val[0] &&
                    s >= lows[i].val[1] && s <= highs[i].val[1] &&
                    v >= lows[i].val[2] && v <= highs[i].val[2]) {
                return true;
            }
        }
        return false;
    }

    private boolean isInRange(double h, double s, double v, Scalar low, Scalar high) {
        return h >= low.val[0] && h <= high.val[0] &&
                s >= low.val[1] && s <= high.val[1] &&
                v >= low.val[2] && v <= high.val[2];
    }


}



public class form {

    static {
        if (!OpenCVLoader.initDebug()) {
            // Handle initialization error
            Log.e("OpenCV", "OpenCV initialization failed");
        } else {
            Log.d("OpenCV", "OpenCV initialized successfully");
        }
    }

    // 中文形状常量
    public static final String TRIANGLE = "三角形";
    public static final String RECTANGLE = "矩形";
    public static final String RHOMBUS = "菱形";
    public static final String STAR = "五角星";
    public static final String UNKNOWN = "未知";

    /**
     * 识别图像中的多个形状并统计数量
     * @param inputBitmap 输入的Bitmap图像
     * @return 包含形状统计的Map（Key为形状名称，Value为数量）
     */
    public static Map<String, Integer> detectShapes(Bitmap inputBitmap) {
        Map<String, Integer> shapeCounts = new HashMap<String, Integer>() {{
            put(TRIANGLE, 0);
            put(RECTANGLE, 0);
            put(RHOMBUS, 0);
            put(STAR, 0);
            put(UNKNOWN, 0);
        }};

        try {
            Mat srcMat = OpencvUtils.transferBitmapToHsvMat(inputBitmap);
            Mat processed = preprocessImage(srcMat);

            List<MatOfPoint> contours = new ArrayList<>();
            Imgproc.findContours(processed, contours, new Mat(),
                    Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

            for (MatOfPoint contour : contours) {
                String shapeType = analyzeContour(contour);
                shapeCounts.put(shapeType, shapeCounts.get(shapeType) + 1);
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        return shapeCounts;
    }

    /**
     * 改进的图像预处理（添加高斯模糊）
     */
    private static Mat preprocessImage(Mat src) {
        // 高斯模糊降噪
        Mat blurred = new Mat();
        Imgproc.GaussianBlur(src, blurred, new Size(5, 5), 0);

        Mat gray = new Mat();
        Imgproc.cvtColor(blurred, gray, Imgproc.COLOR_BGR2GRAY);

        // 自适应阈值二值化
        Mat binary = new Mat();
        Imgproc.adaptiveThreshold(gray, binary, 255,
                Imgproc.ADAPTIVE_THRESH_GAUSSIAN_C,
                Imgproc.THRESH_BINARY, 11, 2);

        // 加强形态学处理
        Mat kernel = Imgproc.getStructuringElement(Imgproc.MORPH_RECT, new Size(3, 3));
        Imgproc.morphologyEx(binary, binary, Imgproc.MORPH_CLOSE, kernel);

        return binary;
    }

    /**
     * 改进的形状分析逻辑
     */
    private static String analyzeContour(MatOfPoint contour) {
        double area = Imgproc.contourArea(contour);
        if (area < 1000) return UNKNOWN; // 提高面积阈值

        MatOfPoint2f contour2f = new MatOfPoint2f(contour.toArray());
        double epsilon = 0.015 * Imgproc.arcLength(contour2f, true);
        MatOfPoint2f approx = new MatOfPoint2f();
        Imgproc.approxPolyDP(contour2f, approx, epsilon, true);

        int vertices = approx.toArray().length;

        // 形状判断逻辑
        if (vertices == 3) {
            return TRIANGLE;
        } else if (vertices == 4) {
            if (isRectangle(approx)) {
                return RECTANGLE;
            } else if (isRhombus(approx)) {
                return RHOMBUS;
            }
        } else if (vertices >= 5 && vertices <= 10) {
            if (isStar(approx)) {
                return STAR;
            }
        }
        return UNKNOWN;
    }

    /**
     * 新增菱形判断（四边长度相等）
     */
    private static boolean isRhombus(MatOfPoint2f approx) {
        Point[] points = approx.toArray();
        if (points.length != 4) return false;

        // 计算四边长度
        double[] sides = new double[4];
        for (int i = 0; i < 4; i++) {
            Point p1 = points[i];
            Point p2 = points[(i+1)%4];
            sides[i] = Math.sqrt(Math.pow(p2.x - p1.x, 2) + Math.pow(p2.y - p1.y, 2));
        }

        // 允许边长最大差异不超过15%
        double avg = (sides[0] + sides[1] + sides[2] + sides[3]) / 4;
        for (double side : sides) {
            if (Math.abs(side - avg) / avg > 0.15) return false;
        }
        return true;
    }

    /**
     * 改进矩形验证（直角验证）
     */
    private static boolean isRectangle(MatOfPoint2f approx) {
        Point[] points = approx.toArray();
        if (points.length != 4) return false;

        double totalAngleError = 0;
        for (int i = 0; i < 4; i++) {
            Point p1 = points[i];
            Point p2 = points[(i+1)%4];
            Point p3 = points[(i+2)%4];
            double angle = Math.toDegrees(getAngle(p1, p2, p3));
            totalAngleError += Math.abs(angle - 90);
        }
        return totalAngleError < 25; // 更严格的角度误差
    }

    /**
     * 五角星判断（凸包面积比）
     */
    private static boolean isStar(MatOfPoint2f approx) {
        MatOfPoint points = new MatOfPoint(approx.toArray());
        MatOfInt hull = new MatOfInt();
        Imgproc.convexHull(points, hull, false);

        double contourArea = Imgproc.contourArea(approx);
        MatOfPoint hullPoints = new MatOfPoint();
        hullPoints.fromList(points.toList().subList(0, hull.rows()));
        double hullArea = Imgproc.contourArea(hullPoints);

        return (hullArea - contourArea) / hullArea > 0.3; // 凹面占比需超过30%
    }


    // 计算三个点的夹角
    private static double getAngle(Point p1, Point p2, Point p3) {
        double a = Math.sqrt(Math.pow(p2.x - p1.x, 2) + Math.pow(p2.y - p1.y, 2));
        double b = Math.sqrt(Math.pow(p3.x - p2.x, 2) + Math.pow(p3.y - p2.y, 2));
        double c = Math.sqrt(Math.pow(p3.x - p1.x, 2) + Math.pow(p3.y - p1.y, 2));
        return Math.acos((a*a + b*b - c*c) / (2 * a * b));
    }
    /**
     * 生成统计结果字符串（示例："Triangles:2, Rectangles:1, Stars:1"）
     */
    public static String getStatisticsString(Map<String, Integer> counts) {
        return String.format("Triangles:%d, Rectangles:%d, Stars:%d",
                counts.get(TRIANGLE),
                counts.get(RECTANGLE),
                counts.get(STAR));
    }

    /**
     * 在原图上绘制所有识别结果
     */
    public static Bitmap drawDetectionResult(Bitmap inputBitmap) {
        try {
            Mat srcMat = OpencvUtils.transferBitmapToHsvMat(inputBitmap);
            Map<String, Integer> counts = detectShapes(inputBitmap);

            // 重新查找并绘制轮廓
            List<MatOfPoint> contours = new ArrayList<>();
            Mat processed = preprocessImage(srcMat);
            Imgproc.findContours(processed, contours, new Mat(),
                    Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

            // 绘制每个轮廓和标签
            int index = 0;
            for (MatOfPoint contour : contours) {
                String shape = analyzeContour(contour);
                if (!shape.equals(UNKNOWN)) {
                    // 绘制轮廓
                    Imgproc.drawContours(srcMat, contours, index,
                            new Scalar(0, 255, 0), 2);

                    // 绘制文字标签
                    Point center = getContourCenter(contour);
                    Imgproc.putText(srcMat, shape, center, 0, 0.6, new Scalar(255, 0, 0), 2);
                }
                index++;
            }

            return OpencvUtils.matToBitmap(srcMat);
        } catch (Exception e) {
            e.printStackTrace();
            return inputBitmap;
        }
    }

    /**
     * 获取轮廓中心点
     */
    private static Point getContourCenter(MatOfPoint contour) {
        Moments m = Imgproc.moments(contour);
        return new Point(m.m10/m.m00, m.m01/m.m00);
    }

    public static String getChineseResult(Map<String, Integer> counts) {
        return String.format("三角形:%d 矩形:%d 菱形:%d 五角星:%d 未知:%d",
                counts.get(TRIANGLE),
                counts.get(RECTANGLE),
                counts.get(RHOMBUS),
                counts.get(STAR),
                counts.get(UNKNOWN));
    }

}