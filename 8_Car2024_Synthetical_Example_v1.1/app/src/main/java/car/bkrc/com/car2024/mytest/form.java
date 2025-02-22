package car.bkrc.com.car2024.mytest;

import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;
import org.opencv.imgproc.Moments;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;


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
/**
 * AdvancedShapeDetector 是一个高级形状检测器类，用于从图像中检测多种几何形状（如圆形、三角形、矩形等）。
 * 它结合了二值化、霍夫圆检测、轮廓查找和多边形逼近等多种OpenCV技术。
 */
public class form {
    // 参数配置
    private int binaryThreshold = 128; // 二值化的阈值，默认为128
    private double houghParam1 = 100; // 霍夫圆检测的第一个参数（Canny边缘检测的高阈值）
    private double houghParam2 = 30; // 霍夫圆检测的第二个参数（累加器阈值）
    private int houghMinRadius = 10; // 霍夫圆检测的最小半径
    private int houghMaxRadius = 300; // 霍夫圆检测的最大半径
    private double minContourArea = 500; // 最小轮廓面积，过滤掉面积过小的轮廓
    private Rect roi = null; // 感兴趣区域（ROI），用于限制检测范围
    private Mat debugImage; // 调试图像，用于绘制检测结果

    /**
     * 构造函数，加载OpenCV库。
     */
    public form() {
        System.loadLibrary("opencv_java3"); // 加载OpenCV的Java库
    }

    /**
     * 设置二值化阈值。
     *
     * @param threshold 二值化的阈值，默认为128。
     */
    public void setBinaryThreshold(int threshold) {
        this.binaryThreshold = threshold;
    }

    /**
     * 设置霍夫圆检测的参数。
     *
     * @param p1 霍夫圆检测的第一个参数（Canny边缘检测的高阈值）。
     * @param p2 霍夫圆检测的第二个参数（累加器阈值）。
     * @param minR 霍夫圆检测的最小半径。
     * @param maxR 霍夫圆检测的最大半径。
     */
    public void setHoughParams(double p1, double p2, int minR, int maxR) {
        houghParam1 = p1;
        houghParam2 = p2;
        houghMinRadius = minR;
        houghMaxRadius = maxR;
    }

    /**
     * 设置感兴趣区域（ROI）。
     *
     * @param roi 感兴趣区域的矩形框。
     */
    public void setROI(Rect roi) {
        this.roi = roi;
    }

    /**
     * 设置最小轮廓面积，过滤掉面积过小的轮廓。
     *
     * @param area 最小轮廓面积。
     */
    public void setMinContourArea(double area) {
        this.minContourArea = area;
    }

    /**
     * 检测图像中的多个形状。
     *
     * @param input 输入的图像（Mat对象）。
     * @return 返回检测到的形状名称列表（如["Circle", "Triangle", "Rectangle"]）。
     */
    public List<String> detectMultiple(Mat input) {
        List<String> results = new ArrayList<>(); // 存储检测结果的列表
        debugImage = input.clone(); // 复制输入图像用于调试绘制

        // 对输入图像应用ROI（如果设置了ROI）
        Mat processed = preprocessImage(applyROI(input));

        // 检测圆形
        detectCircles(processed, results);

        // 检测多边形
        detectPolygons(processed, results);

        return results; // 返回检测到的形状名称列表
    }

    /**
     * 应用感兴趣区域（ROI）。
     *
     * @param input 输入的图像（Mat对象）。
     * @return 返回裁剪后的图像（如果未设置ROI，则返回原图）。
     */
    private Mat applyROI(Mat input) {
        if (roi == null || roi.area() <= 0) { // 如果未设置ROI或ROI无效，返回原图
            return input;
        }
        return new Mat(input, roi); // 返回裁剪后的图像
    }

    /**
     * 图像预处理：包括灰度化、高斯模糊、二值化和形态学闭运算。
     *
     * @param input 输入的图像（Mat对象）。
     * @return 返回预处理后的图像（Mat对象）。
     */
    private Mat preprocessImage(Mat input) {
        Mat gray = new Mat(); // 灰度图像
        Mat blurred = new Mat(); // 高斯模糊后的图像
        Mat binary = new Mat(); // 二值化后的图像

        // 将图像转换为灰度图像
        Imgproc.cvtColor(input, gray, Imgproc.COLOR_BGR2GRAY);

        // 对灰度图像进行高斯模糊
        Imgproc.GaussianBlur(gray, blurred, new Size(5, 5), 0);

        // 对模糊图像进行二值化
        Imgproc.threshold(blurred, binary, binaryThreshold, 255, Imgproc.THRESH_BINARY);

        // 形态学闭运算（去除噪声并填充孔洞）
        Mat kernel = Imgproc.getStructuringElement(Imgproc.MORPH_RECT, new Size(3, 3));
        Imgproc.morphologyEx(binary, binary, Imgproc.MORPH_CLOSE, kernel);

        return binary; // 返回预处理后的图像
    }

    /**
     * 检测图像中的圆形。
     *
     * @param binary 预处理后的二值图像（Mat对象）。
     * @param results 存储检测结果的列表。
     */
    private void detectCircles(Mat binary, List<String> results) {
        Mat circles = new Mat(); // 存储检测到的圆形信息

        // 使用霍夫圆检测算法检测圆形
        Imgproc.HoughCircles(binary, circles, Imgproc.HOUGH_GRADIENT,
                2, 50, houghParam1, houghParam2, houghMinRadius, houghMaxRadius);

        for (int i = 0; i < circles.cols(); i++) { // 遍历检测到的每个圆形
            double[] circle = circles.get(0, i); // 获取圆形信息
            int x = (int) Math.round(circle[0]); // 圆心x坐标
            int y = (int) Math.round(circle[1]); // 圆心y坐标
            int radius = (int) Math.round(circle[2]); // 半径

            results.add("Circle"); // 将圆形添加到结果列表

            // 屏蔽圆形区域以避免重复检测
            Imgproc.circle(binary, new Point(x, y), radius, new Scalar(0), -1);

            // 绘制调试信息（在调试图像上绘制圆形标记）
            drawCircleMarker(debugImage, circle, roi);
        }
    }

    /**
     * 检测图像中的多边形。
     *
     * @param binary 预处理后的二值图像（Mat对象）。
     * @param results 存储检测结果的列表。
     */
    private void detectPolygons(Mat binary, List<String> results) {
        List<MatOfPoint> contours = new ArrayList<>(); // 存储检测到的轮廓
        Imgproc.findContours(binary, contours, new Mat(),
                Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE); // 查找外部轮廓

        for (MatOfPoint contour : contours) { // 遍历每个轮廓
            if (Imgproc.contourArea(contour) < minContourArea) { // 过滤掉面积过小的轮廓
                continue;
            }

            MatOfPoint2f approx = getApproxPolygon(contour); // 获取近似多边形
            String shape = determineShape(approx); // 判断轮廓的形状

            if (!"Unknown".equals(shape)) { // 如果形状已知，添加到结果列表
                results.add(shape);

                // 绘制调试信息（在调试图像上绘制轮廓标记）
                drawContourMarker(debugImage, contour, shape, roi);
            }
        }
    }

    /**
     * 获取轮廓的近似多边形。
     *
     * @param contour 输入的轮廓（MatOfPoint对象）。
     * @return 返回近似多边形（MatOfPoint2f对象）。
     */
    private MatOfPoint2f getApproxPolygon(MatOfPoint contour) {
        MatOfPoint2f contour2f = new MatOfPoint2f(contour.toArray()); // 将轮廓转换为浮点数格式
        double epsilon = 0.03 * Imgproc.arcLength(contour2f, true); // 计算近似精度
        MatOfPoint2f approx = new MatOfPoint2f(); // 存储近似多边形
        Imgproc.approxPolyDP(contour2f, approx, epsilon, true); // 近似多边形
        return approx;
    }

    /**
     * 根据近似多边形判断形状。
     *
     * @param approx 近似多边形（MatOfPoint2f对象）。
     * @return 返回形状名称（如"Circle", "Triangle", "Rectangle"）。
     */
    private String determineShape(MatOfPoint2f approx) {
        int vertexCount = approx.toArray().length; // 获取顶点数量
        double circularity = calculateCircularity(approx); // 计算圆形度

        if (circularity > 0.85 && vertexCount > 8) { // 如果是圆形
            return "Circle";
        }

        switch (vertexCount) { // 根据顶点数量判断形状
            case 3:
                return verifyTriangle(approx) ? "Triangle" : "Unknown"; // 验证是否为三角形
            case 4:
                return verifyQuadrilateral(approx) ? "Rectangle" : "Quadrilateral"; // 验证是否为矩形
            case 5:
                return isPentagram(approx) ? "Pentagram" : "Unknown"; // 验证是否为五角星
            default:
                return "Polygon-" + vertexCount; // 其他多边形
        }
    }

    /**
     * 验证是否为三角形。
     *
     * @param approx 近似多边形（MatOfPoint2f对象）。
     * @return 如果是三角形，则返回true；否则返回false。
     */
    private boolean verifyTriangle(MatOfPoint2f approx) {
        Point[] pts = approx.toArray(); // 获取顶点数组
        if (pts.length != 3) { // 三角形必须有3个顶点
            return false;
        }

        double[] angles = new double[3]; // 存储三个角度
        for (int i = 0; i < 3; i++) {
            Point p1 = pts[i % 3], p2 = pts[(i + 1) % 3], p3 = pts[(i + 2) % 3];
            angles[i] = calculateAngle(p1, p2, p3); // 计算角度
        }

        return Arrays.stream(angles).allMatch(a -> a > 30 && a < 120); // 角度应在合理范围内
    }

    /**
     * 验证是否为矩形。
     *
     * @param approx 近似多边形（MatOfPoint2f对象）。
     * @return 如果是矩形，则返回true；否则返回false。
     */
    private boolean verifyQuadrilateral(MatOfPoint2f approx) {
        Point[] pts = approx.toArray(); // 获取顶点数组
        double[] edgeLengths = new double[4]; // 存储四条边的长度
        for (int i = 0; i < 4; i++) {
            edgeLengths[i] = Math.hypot(pts[i].x - pts[(i + 1) % 4].x, pts[i].y - pts[(i + 1) % 4].y);
        }

        double ratio = Arrays.stream(edgeLengths).max().getAsDouble()
                / Arrays.stream(edgeLengths).min().getAsDouble(); // 计算边长比例

        double[] angles = new double[4]; // 存储四个角度
        for (int i = 0; i < 4; i++) {
            angles[i] = calculateAngle(pts[i], pts[(i + 1) % 4], pts[(i + 2) % 4]);
        }

        return ratio < 1.2 && Arrays.stream(angles).allMatch(a -> Math.abs(a - 90) < 10); // 边长比例接近1，角度接近90度
    }

    /**
     * 判断是否为五角星。
     *
     * @param approx 近似多边形（MatOfPoint2f对象）。
     * @return 如果是五角星，则返回true；否则返回false。
     */
    private boolean isPentagram(MatOfPoint2f approx) {
        if (approx.toArray().length != 5) { // 五角星必须有5个顶点
            return false;
        }
        return !Imgproc.isContourConvex(new MatOfPoint(approx.toArray())); // 五角星是非凸多边形
    }

    /**
     * 计算两个向量之间的夹角。
     *
     * @param p1 第一个点。
     * @param p2 中间点。
     * @param p3 第三个点。
     * @return 返回夹角（单位：度）。
     */
    private double calculateAngle(Point p1, Point p2, Point p3) {
        double aSq = Math.pow(p2.x - p3.x, 2) + Math.pow(p2.y - p3.y, 2);
        double bSq = Math.pow(p1.x - p3.x, 2) + Math.pow(p1.y - p3.y, 2);
        double cSq = Math.pow(p1.x - p2.x, 2) + Math.pow(p1.y - p2.y, 2);
        return Math.toDegrees(Math.acos((bSq + cSq - aSq) / (2 * Math.sqrt(bSq * cSq))));
    }

    /**
     * 计算轮廓的圆形度。
     *
     * @param contour 近似多边形（MatOfPoint2f对象）。
     * @return 返回圆形度（值越接近1，越接近圆形）。
     */
    private double calculateCircularity(MatOfPoint2f contour) {
        double perimeter = Imgproc.arcLength(contour, true); // 计算周长
        double area = Imgproc.contourArea(contour); // 计算面积
        return (4 * Math.PI * area) / (perimeter * perimeter); // 计算圆形度
    }

    /**
     * 调整参数（用于动态调整二值化和霍夫圆检测参数）。
     *
     * @param threshold 二值化阈值。
     * @param param1 霍夫圆检测的第一个参数。
     * @param param2 霍夫圆检测的第二个参数。
     */
    public void adjustParams(int threshold, double param1, double param2) {
        setBinaryThreshold(threshold); // 设置二值化阈值
        setHoughParams(param1, param2, houghMinRadius, houghMaxRadius); // 设置霍夫圆检测参数
    }

    /**
     * 获取调试图像。
     *
     * @return 返回调试图像（Mat对象）。
     */
    public Mat getDebugImage() {
        return debugImage;
    }

    /**
     * 在调试图像上绘制圆形标记。
     *
     * @param image 输入的图像（Mat对象）。
     * @param circle 圆形信息数组（包含圆心x、y坐标和半径）。
     * @param roi 感兴趣区域（可选）。
     */
    private void drawCircleMarker(Mat image, double[] circle, Rect roi) {
        int x = (int) Math.round(circle[0]) + (roi == null ? 0 : roi.x); // 圆心x坐标
        int y = (int) Math.round(circle[1]) + (roi == null ? 0 : roi.y); // 圆心y坐标
        int radius = (int) Math.round(circle[2]); // 半径

        // 在图像上绘制绿色圆形标记
        Imgproc.circle(image, new Point(x, y), radius, new Scalar(0, 255, 0), 2);
    }
    /**
     * 在调试图像上绘制轮廓标记。
     *
     * @param image 输入的图像（Mat对象）。
     * @param contour 轮廓（MatOfPoint对象）。
     * @param label 轮廓的标签（如"Circle", "Triangle"）。
     * @param roi 感兴趣区域（可选）。
     */
    private void drawContourMarker(Mat image, MatOfPoint contour, String label, Rect roi) {
        Moments m = Imgproc.moments(contour); // 计算轮廓的矩
        Point centroid = new Point(m.m10 / m.m00, m.m01 / m.m00); // 计算轮廓的质心

        // 调整质心坐标以考虑ROI偏移
        if (roi != null) {
            centroid.x += roi.x;
            centroid.y += roi.y;
        }
        // 在图像上绘制蓝色轮廓
        Imgproc.drawContours(image, Collections.singletonList(contour), -1, new Scalar(255, 0, 0), 2);
        // 在质心位置绘制标签（红色文字）
        Imgproc.putText(image, label, centroid, Core.FONT_HERSHEY_SIMPLEX, 0.8, new Scalar(0, 0, 255), 2);
    }
}
