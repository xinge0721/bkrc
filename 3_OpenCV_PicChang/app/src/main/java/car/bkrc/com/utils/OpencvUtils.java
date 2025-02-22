package car.bkrc.com.utils;

import android.graphics.Bitmap;
import android.widget.ImageView;

import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

import java.util.ArrayList;
import java.util.List;

/**
* OpencvUtils 是一个工具类，用于处理图像的各种操作，包括二值化、颜色识别、图像转换、膨胀、腐蚀、边缘检测等。
*/
public class OpencvUtils {
   /**
    * 将输入的图像进行二值化处理。
    * 二值化是一种常见的图像处理技术，将图像中的像素值分为两个类别（通常是黑白）。
    * @param src 输入的图像（Mat对象）。
    * @param min 阈值下限，低于此值的像素会被设置为0。
    * @param max 阈值上限，高于此值的像素会被设置为指定的最大值。
    * @return 返回二值化后的图像（Mat对象）。
    * @throws NullPointerException 如果输入的图像对象为空，则抛出异常。
    */
   public static Mat matThreshold(Mat src, int min, int max) {
       if (src == null) {
           throw new NullPointerException("传入的图像对象为空");
       }

       // 创建一个新的Mat对象用于存储灰度图像
       Mat grayMat = new Mat();

       // 将输入图像转换为灰度图像
       Imgproc.cvtColor(src, grayMat, Imgproc.COLOR_BGR2GRAY);

       // 创建一个新的Mat对象用于存储二值化结果
       Mat binary = new Mat();

       // 对灰度图像进行二值化处理
       Imgproc.threshold(grayMat, binary, min, max, Imgproc.THRESH_BINARY);

       return binary; // 返回二值化后的图像
   }

   /**
    * 检测并提取图像中根据HSV筛选后的轮廓，并将结果绘制到指定的ImageView上。
    *
    * @param bitmap 输入的Bitmap图像。
    * @param inrangeMat 已经经过HSV筛选的Mat对象（可选）。
    * @param image_by 显示处理结果的ImageView控件。
    */
   private void updateCanny(Bitmap bitmap, Mat inrangeMat, ImageView image_by) {
       if (bitmap != null) {  // 确保输入图像不为空
           Bitmap bcopy = bitmap.copy(Bitmap.Config.ARGB_8888, true);  // 创建一个可修改的Bitmap副本

           Mat q;  // 用于保存处理后的图像数据

           // 如果提供了inrangeMat，则直接使用；否则从Bitmap转换为HSV格式后再处理
           if (inrangeMat != null) {
               q = OpencvUtils.matCannyRect(bcopy, inrangeMat);  // 使用Canny算法检测边缘并绘制最大轮廓
           } else {
               q = OpencvUtils.matCannyRect(bcopy, OpencvUtils.transferBitmapToHsvMat(bcopy));  // 先转换为HSV格式再进行Canny检测
           }

           Utils.matToBitmap(q, bcopy);  // 将处理后的Mat数据转换回Bitmap
           image_by.setImageBitmap(bcopy);  // 将结果显示在ImageView中
       }
   }
   /**
    * 根据HSV颜色范围筛选图像中的特定颜色区域。
    *
    * @param src 输入的图像（Mat对象）。
    * @param minHsv 最小HSV值数组，表示颜色范围的下限。
    * @param maxHsv 最大HSV值数组，表示颜色范围的上限。
    * @return 返回筛选后的二值图像（Mat对象），其中符合条件的区域为白色，其他区域为黑色。
    * @throws NullPointerException 如果输入的图像对象为空，则抛出异常。
    */
   public static Mat matColorInRange(Mat src, int[] minHsv, int[] maxHsv) {
       if (src == null) {
           throw new NullPointerException("传入的图像对象为空");
       }

       // 创建一个新的Mat对象用于存储HSV格式的图像
       Mat hsv = new Mat();

       // 将输入图像从BGR格式转换为HSV格式
       Imgproc.cvtColor(src, hsv, Imgproc.COLOR_BGR2HSV);

       // 创建一个新的Mat对象用于存储筛选结果
       Mat out = new Mat();

       // 使用Core.inRange方法筛选出符合HSV范围的像素
       Core.inRange(hsv, new Scalar(minHsv[0], minHsv[1], minHsv[2]),
               new Scalar(maxHsv[0], maxHsv[1], maxHsv[2]), out);

       return out; // 返回筛选后的图像
   }

   /**
    * 将Bitmap图像转换为BGR通道的图像。
    *
    * @param bitmap 输入的Bitmap图像。
    * @return 返回转换后的BGR格式图像（Mat对象）。
    * @throws NullPointerException 如果输入的Bitmap对象为空，则抛出异常。
    */
   public static Mat transferBitmapToHsvMat(Bitmap bitmap) {
       Mat src = new Mat();
       if (bitmap == null) {
           throw new NullPointerException("传入的图像对象为空");
       }

       // 将Bitmap转换为Mat对象
       Utils.bitmapToMat(bitmap, src);

       // 创建一个新的Mat对象用于存储BGR格式的图像
       Mat bgr = new Mat();

       // 将RGBA格式的图像转换为BGR格式
       Imgproc.cvtColor(src, bgr, Imgproc.COLOR_RGBA2BGR);

       return bgr; // 返回BGR格式的图像
   }

   /**
    * 对图像进行膨胀操作。
    * 膨胀是形态学操作之一，用于扩大图像中的白色区域。
    *
    * @param src 输入的图像（Mat对象）。
    * @param size 膨胀核的大小，默认为3x3。
    * @param location 膨胀核的锚点位置，默认为中心点。
    * @param iterations 膨胀操作的迭代次数，默认为1次。
    * @return 返回膨胀后的图像（Mat对象）。
    * @throws NullPointerException 如果输入的图像对象为空，则抛出异常。
    */
   public static Mat matDilate(Mat src, Size size, Point location, int iterations) {
       if (src == null) {
           throw new NullPointerException("传入的图像对象为空");
       }

       // 如果未指定膨胀核大小，则默认为3x3
       if (size == null) {
           size = new Size(3, 3);
       }

       // 创建膨胀核
       Mat kernel = Imgproc.getStructuringElement(Imgproc.CV_SHAPE_RECT, size);

       // 创建一个新的Mat对象用于存储膨胀结果
       Mat output = new Mat();

       // 如果未指定锚点位置，则默认为中心点
       if (location == null) {
           location = new Point(-1, -1);
       }

       // 执行膨胀操作
       Imgproc.dilate(src, output, kernel, location, iterations);

       return output; // 返回膨胀后的图像
   }

   /**
    * 简化的图像膨胀操作，默认使用3x3的膨胀核和1次迭代。
    *
    * @param src 输入的图像（Mat对象）。
    * @return 返回膨胀后的图像（Mat对象）。
    */
   public static Mat matDilate(Mat src) {
       return matDilate(src, null, null, 1); // 调用完整的膨胀方法
   }

   /**
    * 对图像进行腐蚀操作。
    * 腐蚀是形态学操作之一，用于缩小图像中的白色区域。
    *
    * @param src 输入的图像（Mat对象）。
    * @param size 腐蚀核的大小，默认为3x3。
    * @param location 腐蚀核的锚点位置，默认为中心点。
    * @param iterations 腐蚀操作的迭代次数，默认为1次。
    * @return 返回腐蚀后的图像（Mat对象）。
    * @throws NullPointerException 如果输入的图像对象为空，则抛出异常。
    */
   public static Mat matErode(Mat src, Size size, Point location, int iterations) {
       if (src == null) {
           throw new NullPointerException("传入的图像对象为空");
       }

       // 如果未指定腐蚀核大小，则默认为3x3
       if (size == null) {
           size = new Size(3, 3);
       }

       // 创建腐蚀核
       Mat kernel = Imgproc.getStructuringElement(Imgproc.CV_SHAPE_RECT, size);

       // 创建一个新的Mat对象用于存储腐蚀结果
       Mat result = new Mat();

       // 如果未指定锚点位置，则默认为中心点
       if (location == null) {
           location = new Point(-1, -1);
       }

       // 执行腐蚀操作
       Imgproc.erode(src, result, kernel, location, iterations);

       return result; // 返回腐蚀后的图像
   }

   /**
    * 简化的图像腐蚀操作，默认使用3x3的腐蚀核和1次迭代。
    *
    * @param src 输入的图像（Mat对象）。
    * @return 返回腐蚀后的图像（Mat对象）。
    */
   public static Mat matErode(Mat src) {
       return matErode(src, null, null, 1); // 调用完整的腐蚀方法
   }

   /**
    * 检测图像中的边缘，并绘制最大轮廓。
    *
    * @param last 用于绘制轮廓的原始图像（Bitmap对象）。
    * @param src 输入的图像（Mat对象）。
    * @return 返回带有绘制轮廓的图像（Mat对象）。
    * @throws NullPointerException 如果输入的图像对象为空，则抛出异常。
    */
   public static Mat matCannyRect(Bitmap last, Mat src) {
       if (src == null) {
           throw new NullPointerException("传入的图像对象为空");
       }

       // 克隆输入图像以避免修改原始数据
       Mat mat = src.clone();

       // 使用Canny算法检测图像中的边缘
       Imgproc.Canny(src, mat, 75, 200);

       // 创建一个列表用于存储找到的轮廓
       List<MatOfPoint> contours = new ArrayList<>();

       // 创建一个Mat对象用于存储层次结构信息
       Mat hierarchy = new Mat();

       // 寻找轮廓
       Imgproc.findContours(mat, contours, hierarchy, Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

       // 初始化变量用于记录最大轮廓的索引和周长
       int index = 0;
       double perimeter = 0;

       // 遍历所有找到的轮廓，寻找具有最大周长的轮廓
       for (int i = 0; i < contours.size(); i++) {
           MatOfPoint2f source = new MatOfPoint2f();
           source.fromList(contours.get(i).toList());
           double length = Imgproc.arcLength(source, true); // 计算轮廓的周长
           if (length > perimeter) {
               perimeter = length;
               index = i; // 更新最大轮廓的索引
           }
       }

       // 将Bitmap转换为Mat对象，用于绘制轮廓
       Mat lastMat = new Mat();
       Utils.bitmapToMat(last, lastMat);

       // 在图像上绘制最大轮廓
       Imgproc.drawContours(
               lastMat, // 目标图像
               contours, // 轮廓集合
               index, // 要绘制的轮廓索引
               new Scalar(255, 0, 0), // 绘制颜色（蓝色）
               5, // 线条粗细
               Imgproc.LINE_AA // 抗锯齿线条类型
       );

       return lastMat; // 返回带有绘制轮廓的图像
   }
    /**
     * 将 Bitmap 转换为 Mat。
     *
     * @param bitmap 输入的 Bitmap 对象。
     * @return 返回对应的 Mat 对象。
     */
    public static Mat bitmapToMat(Bitmap bitmap) {
        if (bitmap == null) {
            throw new IllegalArgumentException("输入的 Bitmap 不能为空！");
        }

        // 创建一个 Mat 对象
        Mat mat = new Mat();

        // 使用 OpenCV 提供的 Utils.bitmapToMat 方法进行转换
        Utils.bitmapToMat(bitmap, mat);

        return mat;
    }

    /**
     * 将 Mat 转换为 Bitmap。
     *
     * @param mat 输入的 Mat 对象。
     * @return 返回对应的 Bitmap 对象。
     */
    public static Bitmap matToBitmap(Mat mat) {
        if (mat == null) {
            throw new IllegalArgumentException("输入的 Mat 不能为空！");
        }

        // 创建一个 Bitmap 对象
        Bitmap bitmap = Bitmap.createBitmap(mat.cols(), mat.rows(), Bitmap.Config.ARGB_8888);

        // 使用 OpenCV 提供的 Utils.matToBitmap 方法进行转换
        Utils.matToBitmap(mat, bitmap);

        return bitmap;
    }

}
