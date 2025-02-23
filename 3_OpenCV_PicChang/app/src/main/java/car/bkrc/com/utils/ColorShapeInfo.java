package car.bkrc.com.utils;

import androidx.annotation.NonNull;

public class ColorShapeInfo {
    public final String color;
    public final String shape;

    public ColorShapeInfo(String color, String shape) {
        this.color = color;
        this.shape = shape;
    }

    @NonNull
    @Override
    public String toString() {
        return color + "色-" + shape;
    }
}