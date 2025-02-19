package car.bkrc.com.car2024.FragmentView;


import android.content.Context;
import android.graphics.Bitmap;  // 导入Bitmap类，用于图像的处理
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.util.Log;
import android.widget.ImageView;  // 导入ImageView类，用于显示图像



import org.opencv.android.Utils;  // 导入OpenCV的工具类，用于Mat与Bitmap之间的转换
import org.opencv.core.Mat;  // 导入Mat类，OpenCV中的图像矩阵类

import org.opencv.core.Core;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;
import org.opencv.core.*;
import org.opencv.imgcodecs.Imgcodecs;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

class OpencvUtils {

    /**
     * 将Bitmap进行二值化
     * */
    public static Mat matThreshold(Mat src, int min, int max){

        if(src == null){
            throw new NullPointerException("传入的图像对象为空");
        }

        Mat grayMat = new Mat();
        //先将图像进行灰度化
        Imgproc.cvtColor(src,grayMat,Imgproc.COLOR_BGR2GRAY);

        //二值化图像
        Mat binary = new Mat();

        //将灰度化的图像进行颜色处理
        Imgproc.threshold(grayMat,binary,min,max,Imgproc.THRESH_BINARY);
        return binary;

    }


    /**
     * 将Bitmap 根据传入的HSV进行识别
     * */
    public static Mat matColorInRange(Mat src,int[] minHsv, int[] maxHsv){
        if(src == null){
            throw new NullPointerException("传入的图像对象为空");
        }
        Mat hsv = new Mat();
        Imgproc.cvtColor(src,hsv,Imgproc.COLOR_BGR2HSV);
        Mat out = new Mat();
        Core.inRange(hsv,new Scalar(minHsv[0],minHsv[1],minHsv[2]),new Scalar(maxHsv[0],maxHsv[1],maxHsv[2]),out);
        return out;


    }

    /**
     * 将Bitmap图像转换为 BGR通道的图像
     * */
    public static Mat transferBitmapToHsvMat(Bitmap bitmap){
        Mat src = new Mat();
        if(bitmap == null){
            throw new NullPointerException("传入的图像对象为空");
        }
        Utils.bitmapToMat(bitmap,src);
        Mat bgr = new Mat();
        //android上的是RGBA，所以先转成BGR
        Imgproc.cvtColor(src,bgr,Imgproc.COLOR_RGBA2BGR);


        return bgr;


    }

    /**
     * 图像的膨胀操作
     * @param src 传入的BGR格式的图像
     * @param size 膨胀的范围 可以设置为new Size(3,3) 可对这里的参数进行更改 可填null
     * @param location 膨胀的位置 可填null 参考 new Point(-1,-1)
     * @param iterations 膨胀次数
     *
     */
    public static Mat matDilate(Mat src, Size size, Point location,int iterations){
        if(src == null){
            throw new NullPointerException("传入的图像对象为空");
        }
        //图像膨胀
        if(size == null){
            size = new Size(3,3);
        }
        Mat kernel = Imgproc.getStructuringElement(Imgproc.CV_SHAPE_RECT,size);
        Mat output = new Mat();
        if(location == null){
            location = new Point(-1,-1);
        }
        Imgproc.dilate(src, output, kernel,location,iterations);
        return output;

    }
    /**
     * 图像的膨胀操作
     * @param src 传入的BGR格式的图像
     *
     */
    public static Mat matDilate(Mat src){
        return matDilate(src,null,null,1);

    }


    /**
     * 图像的腐蚀操作
     * @param src 传入的BGR格式的图像
     * @param size 腐蚀的范围 可以设置为new Size(3,3) 可对这里的参数进行更改 可填null
     * @param location 腐蚀的位置 可填null 参考 new Point(-1,-1)
     * @param iterations 腐蚀次数
     *
     */
    public static Mat matErode(Mat src, Size size, Point location,int iterations){
        if(src == null){
            throw new NullPointerException("传入的图像对象为空");
        }
        //图像腐蚀
        if(size == null){
            size = new Size(3,3);
        }

        Mat kernel = Imgproc.getStructuringElement(Imgproc.CV_SHAPE_RECT,size);
        Mat result = new Mat();
        if(location == null){
            location = new Point(-1,-1);
        }
        Imgproc.erode(src, result, kernel, location,iterations);

        return result;
    }

    /**
     * 图像的腐蚀操作
     * @param src 传入的BGR格式的图像
     *
     */
    public static Mat matErode(Mat src){
        return matErode(src,null,null,1);

    }


    /**
     * 检测边缘
     * */
    public static Mat matCannyRect(Bitmap last,Mat src){
        if(src == null){
            throw new NullPointerException("传入的图像对象为空");
        }

        Mat mat = src.clone();
        Imgproc.Canny(src, mat, 75, 200);

        List<MatOfPoint> contours = new ArrayList<MatOfPoint>();
        Mat hierarchy = new Mat();
        // 寻找轮廓
        Imgproc.findContours(mat, contours, hierarchy, Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);
        int index = 0;
        double perimeter = 0;
        // 找出匹配到的最大轮廓
        for (int i = 0; i < contours.size(); i++) {
            // 最大面积
//            double area = Imgproc.contourArea(contours.get(i));
            //最大周长
            MatOfPoint2f source = new MatOfPoint2f();
            source.fromList(contours.get(i).toList());
            double length = Imgproc.arcLength(source,true);
            if(length>perimeter){
                perimeter =  length;
                index = i;
            }
        }

        Mat lastMat = new Mat();
        Utils.bitmapToMat(last,lastMat);
        /*
         * 参数一：image，待绘制轮廓的图像。
         *
         * 参数二：contours，待绘制的轮廓集合。
         *
         * 参数三：contourIdx，要绘制的轮廓在contours中的索引，若为负数，表示绘制全部轮廓。
         *
         * 参数四：color，绘制轮廓的颜色。
         *
         * 参数五：thickness，绘制轮廓的线条粗细。若为负数，那么绘制轮廓的内部。
         *
         * 参数六：lineType，线条类型。FILLED   LINE_4   4连通   LINE_8   8连通  LINE_AA  抗锯齿
         */
        Imgproc.drawContours(
                lastMat,
                contours,
                index,
                new Scalar(255,0, 0),
                5,
                Imgproc.LINE_AA

        );
        return lastMat;



    }
}

public class myOpencv {  // 声明一个名为opencv的公共类
    private Bitmap bitmap = null;  // 声明一个Bitmap对象用于保存图像数据，初始化为空
    public Mat inrangeMat = null;  // 声明一个Mat对象用于保存图像的处理结果，初始化为空


    /**
     * 检测根据hsv 提取到图像的轮廓
     */
    private void updateCanny(Bitmap bitmap, Mat inrangeMat, ImageView image_by) {  // 声明一个更新Canny边缘检测的私有方法，接收Bitmap图像、Mat图像数据和ImageView作为参数
        if (bitmap != null) {  // 如果传入的Bitmap图像不为空
            Bitmap bcopy = bitmap.copy(Bitmap.Config.ARGB_8888, true);  // 创建一个可修改的Bitmap副本

            Mat q;  // 声明一个Mat变量，用于保存处理后的图像

            if (inrangeMat != null) {  // 如果传入的inrangeMat不为空
                q = OpencvUtils.matCannyRect(bcopy, inrangeMat);  // 使用OpencvUtils工具类的matCannyRect方法进行Canny边缘检测
            } else {  // 如果inrangeMat为空
                q = OpencvUtils.matCannyRect(bcopy, OpencvUtils.transferBitmapToHsvMat(bcopy));  // 将Bitmap转换为HSV格式，然后进行Canny边缘检测
            }

            Utils.matToBitmap(q, bcopy);  // 将处理后的Mat数据转换回Bitmap并存入bcopy
            image_by.setImageBitmap(bcopy);  // 将处理后的Bitmap显示在传入的ImageView中
        }
    }

    public Mat blurMat;  // 声明一个Mat对象用于保存模糊处理后的图像
    public int b_min = 70, b_max = 120;  // 声明两个整数变量，用于控制二值化的最小值和最大值

    /**
     * 对图像进行二值化
     */
    public void updateBlur(Bitmap bitmap, ImageView image_blur) {  // 声明一个更新图像模糊处理的公共方法，接收Bitmap图像和ImageView作为参数
        if (bitmap != null) {  // 如果传入的Bitmap图像不为空
            Bitmap bcopy = bitmap.copy(Bitmap.Config.ARGB_8888, true);  // 创建一个可修改的Bitmap副本
            blurMat = OpencvUtils.matThreshold(OpencvUtils.transferBitmapToHsvMat(bcopy), b_min, b_max);  // 将Bitmap转换为HSV格式，然后进行二值化处理
            Utils.matToBitmap(blurMat, bcopy);  // 将处理后的Mat数据转换回Bitmap并存入bcopy
            image_blur.setImageBitmap(bcopy);  // 将处理后的Bitmap显示在传入的ImageView中
        }
    }

    /**
     * 对图像进行一次腐蚀操作
     */
    public void updateErode(Bitmap bitmap, Mat inrangeMat, ImageView image_hsv) {  // 声明一个更新图像腐蚀操作的公共方法，接收Bitmap图像、Mat图像数据和ImageView作为参数
        if (bitmap != null) {  // 如果传入的Bitmap图像不为空
            Bitmap bcopy = bitmap.copy(Bitmap.Config.ARGB_8888, true);  // 创建一个可修改的Bitmap副本

            if (inrangeMat != null) {  // 如果传入的inrangeMat不为空
                inrangeMat = OpencvUtils.matErode(inrangeMat);  // 使用OpencvUtils工具类的matErode方法进行腐蚀操作
            } else {  // 如果inrangeMat为空
                inrangeMat = OpencvUtils.matErode(OpencvUtils.transferBitmapToHsvMat(bcopy));  // 将Bitmap转换为HSV格式，然后进行腐蚀操作
            }
            Utils.matToBitmap(inrangeMat, bcopy);  // 将处理后的Mat数据转换回Bitmap并存入bcopy
            image_hsv.setImageBitmap(bcopy);  // 将处理后的Bitmap显示在传入的ImageView中
        }
    }

    /**
     * 对图像进行一次膨胀操作
     */
    public void updateDilate(Bitmap bitmap, Mat inrangeMat, ImageView image_hsv) {  // 声明一个更新图像膨胀操作的公共方法，接收Bitmap图像、Mat图像数据和ImageView作为参数
        if (bitmap != null) {  // 如果传入的Bitmap图像不为空
            Bitmap bcopy = bitmap.copy(Bitmap.Config.ARGB_8888, true);  // 创建一个可修改的Bitmap副本
            if (inrangeMat != null) {  // 如果传入的inrangeMat不为空
                inrangeMat = OpencvUtils.matDilate(inrangeMat);  // 使用OpencvUtils工具类的matDilate方法进行膨胀操作
            } else {  // 如果inrangeMat为空
                inrangeMat = OpencvUtils.matDilate(OpencvUtils.transferBitmapToHsvMat(bcopy));  // 将Bitmap转换为HSV格式，然后进行膨胀操作
            }

            Utils.matToBitmap(inrangeMat, bcopy);  // 将处理后的Mat数据转换回Bitmap并存入bcopy
            image_hsv.setImageBitmap(bcopy);  // 将处理后的Bitmap显示在传入的ImageView中
        }
    }

    /**
     * 根据HSV查找色块
     */
    public int hmin = 0, hmax = 0, smin = 0, smax = 0, vmin = 0, vmax = 0;  // 声明6个整数变量，用于设置HSV色调范围的最小值和最大值

    public void updateHsv(Bitmap bitmap, Mat inrangeMat, ImageView image_hsv) {  // 声明一个根据HSV查找色块的公共方法，接收Bitmap图像、Mat图像数据和ImageView作为参数
        if (bitmap != null) {  // 如果传入的Bitmap图像不为空
            Bitmap bcopy = bitmap.copy(Bitmap.Config.ARGB_8888, true);  // 创建一个可修改的Bitmap副本

            inrangeMat = OpencvUtils.matColorInRange(OpencvUtils.transferBitmapToHsvMat(bcopy), new int[]{hmin, smin, vmin}, new int[]{hmax, smax, vmax});  // 将Bitmap转换为HSV格式，并根据设置的HSV范围提取色块
            Utils.matToBitmap(inrangeMat, bcopy);  // 将处理后的Mat数据转换回Bitmap并存入bcopy
            image_hsv.setImageBitmap(bcopy);  // 将处理后的Bitmap显示在传入的ImageView中
        }
    }

    public int[] colorData = new int[3]; // 存储颜色数据
    /**
     * 图像去色处理（提取高亮区域）
     */
    public Bitmap decolouring(Bitmap recbitmap) {
        int width = recbitmap.getWidth();
        int height = recbitmap.getHeight();
        Bitmap grayBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int pixel = recbitmap.getPixel(x, y);
                int r = Color.red(pixel);
                int g = Color.green(pixel);
                int b = Color.blue(pixel);

                // 保留高亮区域（白色或其他亮色）
                if (r > 200 && g > 200 && b > 200) {
                    grayBitmap.setPixel(x, y, Color.rgb(255, 255, 255));
                } else {
                    grayBitmap.setPixel(x, y, Color.rgb(0, 0, 0));
                }
            }
        }
        return grayBitmap;
    }
    /**
     * 颜色识别（统计红、绿、黄色块）
     */
    public int[] detectColor(Bitmap bitmap) {
        colorData = new int[] {0, 0, 0};
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int pixel = bitmap.getPixel(x, y);
                int r = Color.red(pixel);
                int g = Color.green(pixel);
                int b = Color.blue(pixel);

                if (r > 200 && g < 50 && b < 50) { // 红色
                    colorData[0]++;
                } else if (g > 200 && r < 50 && b < 50) { // 绿色
                    colorData[1]++;
                } else if (r > 200 && g > 200 && b < 50) { // 黄色
                    colorData[2]++;
                }
            }
        }
        return colorData;
    }




    /**
     * 将本地图片资源转换为Bitmap
     * @param context 上下文
     * @param vectorDrawableId 图片资源ID
     * @return 转换后的Bitmap
     */
    private static Bitmap getBitmap(Context context, int vectorDrawableId) {
        // 兼容不同Android版本加载矢量图
        if (Build.VERSION.SDK_INT > Build.VERSION_CODES.LOLLIPOP) {
            Drawable vectorDrawable = context.getDrawable(vectorDrawableId);
            Bitmap bitmap = Bitmap.createBitmap(vectorDrawable.getIntrinsicWidth(), vectorDrawable.getIntrinsicHeight(), Bitmap.Config.ARGB_8888);
            Canvas canvas = new Canvas(bitmap);
            vectorDrawable.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
            vectorDrawable.draw(canvas);
            return bitmap;
        } else {
            return BitmapFactory.decodeResource(context.getResources(), vectorDrawableId);
        }
    }


    /** 核心调用接口 */
    public static String detect(Bitmap input) {
        ShapeColorDetector detector = new ShapeColorDetector();
        return detector.processBitmap(input);
    }

    /** 文件路径版本的重载 */
    public static String detect(String imagePath) {
        ShapeColorDetector detector = new ShapeColorDetector();
        return detector.processImage(imagePath);
    }


}

class ShapeColorDetector {
    private static final double COLOR_THRESHOLD = 50;
    private static final double CIRCULARITY_THRESHOLD = 0.78;
    private static final double MIN_CONTOUR_AREA = 100;
//    定义了各个颜色
    private final Map<String, Scalar> colorMap = new HashMap<String, Scalar>() {{
        put("red", new Scalar(0, 0, 255));
        put("green", new Scalar(0, 255, 0));
        put("blue", new Scalar(255, 0, 0));
        put("yellow", new Scalar(0, 255, 255));
        put("magenta", new Scalar(255, 0, 255));
        put("cyan", new Scalar(255, 255, 0));
        put("black", new Scalar(0, 0, 0));
    }};

    public String processBitmap(Bitmap bitmap) {
        Mat src = new Mat();
        Utils.bitmapToMat(bitmap, src);
        Imgproc.cvtColor(src, src, Imgproc.COLOR_RGBA2BGR); // ▲ 颜色转换

        if (src.empty()) return "Error loading image";

        Mat processed = preprocessImage(src);
        List<MatOfPoint> contours = findContours(processed);

        int[] counts = new int[5];

        for (MatOfPoint contour : contours) {
            if (Imgproc.contourArea(contour) < MIN_CONTOUR_AREA) continue;

            String shape = detectShape(contour);
            String color = detectColor(src, contour);

            if (shape != null && color != null) {
                switch (shape) {
                    case "rectangle": counts[0]++; break;
                    case "circle": counts[1]++; break;
                    case "triangle": counts[2]++; break;
                    case "diamond": counts[3]++; break;
                    case "star": counts[4]++; break;
                }
            }
            contour.release(); // ▼ 循环内释放每个contour的内存
        }
    // ▼ 强制释放所有OpenCV对象
    src.release();
    processed.release();

//    Log.d("COLOR_DEBUG", "BGR均值 → B:" + mean.val[0] + " G:" + mean.val[1] + " R:" + mean.val[2]);

    return String.format("a:%d,b:%d,c:%d,d:%d,e:%d", counts[0], counts[1], counts[2], counts[3], counts[4]);
}
//    形状识别函数关键
    public String processImage(String imagePath) {
        Mat src = Imgcodecs.imread(imagePath);
        if (src.empty()) return "Error loading image";

        Mat processed = preprocessImage(src);
        List<MatOfPoint> contours = findContours(processed);

        int[] counts = new int[5]; // a,b,c,d,e

        for (MatOfPoint contour : contours) {
            if (Imgproc.contourArea(contour) < MIN_CONTOUR_AREA) continue;

            String shape = detectShape(contour);
            String color = detectColor(src, contour);

            if (shape != null && color != null) {
                switch (shape) {
                    case "rectangle": counts[0]++; break;
                    case "circle": counts[1]++; break;
                    case "triangle": counts[2]++; break;
                    case "diamond": counts[3]++; break;
                    case "star": counts[4]++; break;
                }
            }
        }

        return String.format("a:%d,b:%d,c:%d,d:%d,e:%d",
                counts[0], counts[1], counts[2], counts[3], counts[4]);
    }

    private Mat preprocessImage(Mat src) {
        Mat gray = new Mat();
        Mat blurred = new Mat();
        Mat edges = new Mat();

        Imgproc.cvtColor(src, gray, Imgproc.COLOR_BGR2GRAY);
        Imgproc.GaussianBlur(gray, blurred, new Size(5, 5), 0);
        Imgproc.Canny(blurred, edges, 50, 150);

        return edges;
    }

    private List<MatOfPoint> findContours(Mat edges) {
        List<MatOfPoint> contours = new ArrayList<>();
        Mat hierarchy = new Mat();
        Imgproc.findContours(edges, contours, hierarchy,
                Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);
        return contours;
    }

    private String detectShape(MatOfPoint contour) {
        MatOfPoint2f approx = new MatOfPoint2f();
        MatOfPoint2f contour2f = new MatOfPoint2f(contour.toArray());

        double epsilon = 0.02 * Imgproc.arcLength(contour2f, true);
        Imgproc.approxPolyDP(contour2f, approx, epsilon, true);

        int vertices = approx.toArray().length;

        if (vertices == 3) return "triangle";
        if (vertices == 4) return detectQuadrilateral(contour, approx);
        if (vertices > 4 && isStar(contour)) return "star";

        return isCircle(contour) ? "circle" : null;
    }

    private String detectQuadrilateral(MatOfPoint contour, MatOfPoint2f approx) {
        Rect rect = Imgproc.boundingRect(contour);
        double ratio = (double)rect.width/rect.height;

        // 正方形判断
        if (ratio >= 0.95 && ratio <= 1.05) return "rectangle";

        // 菱形判断
        Point[] points = approx.toArray();
        double[] lengths = new double[4];
        for (int i=0; i<4; i++) {
            Point p1 = points[i], p2 = points[(i+1)%4];
            lengths[i] = Math.sqrt(Math.pow(p2.x-p1.x, 2) + Math.pow(p2.y-p1.y, 2));
        }

        boolean isRhombus = true;
        for (int i=1; i<4; i++) {
            if (Math.abs(lengths[i]-lengths[0])/lengths[0] > 0.15) {
                isRhombus = false;
                break;
            }
        }
        return isRhombus ? "diamond" : "rectangle";
    }

    private boolean isCircle(MatOfPoint contour) {
        Point center = new Point();
        float[] radius = new float[1];
        Imgproc.minEnclosingCircle(new MatOfPoint2f(contour.toArray()), center, radius);

        double circleArea = Math.PI * Math.pow(radius[0], 2);
        double contourArea = Imgproc.contourArea(contour);
        return (contourArea/circleArea) > CIRCULARITY_THRESHOLD;
    }

    private boolean isStar(MatOfPoint contour) {
        MatOfInt hull = new MatOfInt();
        Imgproc.convexHull(contour, hull);

        MatOfInt4 defects = new MatOfInt4();
        Imgproc.convexityDefects(contour, hull, defects);

        // ▼ 正确提取二维数组形态的凸缺陷数据
        List<Integer> defectList = defects.toList(); // 这里实际得到一维的整数列表
        int validDefects = 0;

        // ▶ 改为按每4个元素为一组遍历（缺陷数据存储结构为连续的4个整数）
        for (int i = 0; i < defectList.size(); i += 4) {
            // ▲ 重要提示：每个缺陷由连续的4个int值构成
            int depth = defectList.get(i + 3); // 第4个元素是深度值
            if (depth / 256f > 20) { // 深度值需要除以256还原实际像素距离
                validDefects++;
            }
        }
        return validDefects >= 5;
    }


    private String detectColor(Mat src, MatOfPoint contour) {
        Mat mask = Mat.zeros(src.size(), CvType.CV_8UC1);
        Imgproc.drawContours(mask, List.of(contour), -1, new Scalar(255), -1);

        Scalar mean = Core.mean(src, mask);
        return findNearestColor(new double[]{mean.val[0], mean.val[1], mean.val[2]});
    }

    private String findNearestColor(double[] bgr) {
        String closest = null;
        double minDist = Double.MAX_VALUE;

        for (Map.Entry<String, Scalar> entry : colorMap.entrySet()) {
            double[] target = entry.getValue().val;
            double dist = Math.sqrt(
                    Math.pow(bgr[0]-target[0], 2) +
                            Math.pow(bgr[1]-target[1], 2) +
                            Math.pow(bgr[2]-target[2], 2));

            if (dist < minDist && dist < COLOR_THRESHOLD) {
                minDist = dist;
                closest = entry.getKey();
            }
        }
        return closest;
    }
}