package car.bkrc.com.car2024.mytest;


import android.content.Context;
import android.graphics.Bitmap;  // 导入Bitmap类，用于图像的处理
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.widget.ImageView;  // 导入ImageView类，用于显示图像



import org.opencv.android.Utils;  // 导入OpenCV的工具类，用于Mat与Bitmap之间的转换
import org.opencv.core.Mat;  // 导入Mat类，OpenCV中的图像矩阵类



/**
 * myOpencv 是一个封装了多种图像处理功能的类，包括边缘检测、二值化、腐蚀、膨胀、HSV颜色提取、颜色识别等。
 */
public class myOpencv {



    public Mat blurMat;  // 保存模糊处理后的图像数据
    public int b_min = 70, b_max = 120;  // 设置二值化的阈值范围

    /**
     * 对输入的Bitmap图像进行二值化处理，并将结果显示在指定的ImageView上。
     *
     * @param bitmap 输入的Bitmap图像。
     * @param image_blur 显示处理结果的ImageView控件。
     */
    public void updateBlur(Bitmap bitmap, ImageView image_blur) {
        if (bitmap != null) {  // 确保输入图像不为空
            Bitmap bcopy = bitmap.copy(Bitmap.Config.ARGB_8888, true);  // 创建一个可修改的Bitmap副本

            blurMat = OpencvUtils.matThreshold(  // 进行二值化处理
                    OpencvUtils.transferBitmapToHsvMat(bcopy),  // 先将Bitmap转换为HSV格式
                    b_min, b_max  // 使用设置的阈值范围
            );

            Utils.matToBitmap(blurMat, bcopy);  // 将处理后的Mat数据转换回Bitmap
            image_blur.setImageBitmap(bcopy);  // 将结果显示在ImageView中
        }
    }
    public int hmin = 0, hmax = 0, smin = 0, smax = 0, vmin = 0, vmax = 0;  // HSV色调范围的最小值和最大值

    /**
     * 根据设置的HSV范围提取图像中的特定颜色区域，并将结果显示在指定的ImageView上。
     *
     * @param bitmap 输入的Bitmap图像。
     * @param inrangeMat 已经经过处理的Mat对象（可选）。
     * @param image_hsv 显示处理结果的ImageView控件。
     */
    public void updateHsv(Bitmap bitmap, Mat inrangeMat, ImageView image_hsv) {
        if (bitmap != null) {  // 确保输入图像不为空
            Bitmap bcopy = bitmap.copy(Bitmap.Config.ARGB_8888, true);  // 创建一个可修改的Bitmap副本

            inrangeMat = OpencvUtils.matColorInRange(  // 根据HSV范围提取颜色区域
                    OpencvUtils.transferBitmapToHsvMat(bcopy),  // 先将Bitmap转换为HSV格式
                    new int[]{hmin, smin, vmin},  // 最小HSV值
                    new int[]{hmax, smax, vmax}  // 最大HSV值
            );

            Utils.matToBitmap(inrangeMat, bcopy);  // 将处理后的Mat数据转换回Bitmap
            image_hsv.setImageBitmap(bcopy);  // 将结果显示在ImageView中
        }
    }

    public int[] colorData = new int[3];  // 存储颜色统计结果（红、绿、黄）

    /**
     * 对输入的Bitmap图像进行去色处理，提取高亮区域（白色或其他亮色）。
     *
     * @param recbitmap 输入的Bitmap图像。
     * @return 返回处理后的Bitmap图像。
     */
    public Bitmap decolouring(Bitmap recbitmap) {
        int width = recbitmap.getWidth();  // 获取图像宽度
        int height = recbitmap.getHeight();  // 获取图像高度
        Bitmap grayBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);  // 创建一个新的Bitmap用于存储结果

        for (int y = 0; y < height; y++) {  // 遍历每一行像素
            for (int x = 0; x < width; x++) {  // 遍历每一列像素
                int pixel = recbitmap.getPixel(x, y);  // 获取当前像素值
                int r = Color.red(pixel);  // 提取红色通道值
                int g = Color.green(pixel);  // 提取绿色通道值
                int b = Color.blue(pixel);  // 提取蓝色通道值

                // 如果像素是高亮区域（白色或其他亮色），则保留为白色；否则设置为黑色
                if (r > 200 && g > 200 && b > 200) {
                    grayBitmap.setPixel(x, y, Color.rgb(255, 255, 255));
                } else {
                    grayBitmap.setPixel(x, y, Color.rgb(0, 0, 0));
                }
            }
        }
        return grayBitmap;  // 返回处理后的图像
    }

    /**
     * 统计输入Bitmap图像中红色、绿色和黄色块的数量。
     *
     * @param bitmap 输入的Bitmap图像。
     * @return 返回一个长度为3的数组，分别表示红色、绿色和黄色块的数量。
     */
    public int[] detectColor(Bitmap bitmap) {
        colorData = new int[]{0, 0, 0};  // 初始化颜色统计数据

        int width = bitmap.getWidth();  // 获取图像宽度
        int height = bitmap.getHeight();  // 获取图像高度

        for (int y = 0; y < height; y++) {  // 遍历每一行像素
            for (int x = 0; x < width; x++) {  // 遍历每一列像素
                int pixel = bitmap.getPixel(x, y);  // 获取当前像素值
                int r = Color.red(pixel);  // 提取红色通道值
                int g = Color.green(pixel);  // 提取绿色通道值
                int b = Color.blue(pixel);  // 提取蓝色通道值

                // 判断像素属于哪种颜色，并更新对应的计数器
                if (r > 200 && g < 50 && b < 50) {  // 红色
                    colorData[0]++;
                } else if (g > 200 && r < 50 && b < 50) {  // 绿色
                    colorData[1]++;
                } else if (r > 200 && g > 200 && b < 50) {  // 黄色
                    colorData[2]++;
                }
            }
        }
        return colorData;  // 返回颜色统计数据
    }

    /**
     * 将本地图片资源转换为Bitmap对象。
     *
     * @param context 应用程序上下文。
     * @param vectorDrawableId 图片资源ID。
     * @return 返回转换后的Bitmap对象。
     */
    private static Bitmap getBitmap(Context context, int vectorDrawableId) {
        if (Build.VERSION.SDK_INT > Build.VERSION_CODES.LOLLIPOP) {  // 兼容不同Android版本加载矢量图
            Drawable vectorDrawable = context.getDrawable(vectorDrawableId);  // 加载矢量图
            Bitmap bitmap = Bitmap.createBitmap(  // 创建一个与矢量图大小相同的Bitmap
                    vectorDrawable.getIntrinsicWidth(),
                    vectorDrawable.getIntrinsicHeight(),
                    Bitmap.Config.ARGB_8888
            );
            Canvas canvas = new Canvas(bitmap);  // 创建一个Canvas对象
            vectorDrawable.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());  // 设置矢量图的边界
            vectorDrawable.draw(canvas);  // 将矢量图绘制到Canvas上
            return bitmap;
        } else {
            return BitmapFactory.decodeResource(context.getResources(), vectorDrawableId);  // 直接解码资源为Bitmap
        }
    }


}

