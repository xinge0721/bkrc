package car.bkrc.com.utils;

import android.graphics.Bitmap;

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

public class OpencvUtils {

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
