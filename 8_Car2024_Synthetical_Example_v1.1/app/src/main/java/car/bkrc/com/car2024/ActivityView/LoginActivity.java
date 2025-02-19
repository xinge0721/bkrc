package car.bkrc.com.car2024.ActivityView;

import android.Manifest; // 引入权限相关的类
import android.annotation.SuppressLint; // 引入注解用于控制代码检查
import android.app.ProgressDialog; // 引入进度对话框类
import android.content.Context; // 引入上下文类
import android.content.Intent; // 引入意图类，用于启动活动
import android.content.SharedPreferences; // 引入共享首选项，用于存储数据
import android.content.pm.PackageManager; // 引入包管理类
import android.content.res.Configuration; // 引入配置类，用于获取设备屏幕配置
import android.net.wifi.WifiInfo; // 引入Wi-Fi信息类
import android.net.wifi.WifiManager; // 引入Wi-Fi管理类
import android.os.Build; // 引入构建信息类，用于判断系统版本
import android.os.Bundle; // 引入Bundle类，用于传递数据
import android.os.Vibrator; // 引入震动管理类
import android.text.method.HideReturnsTransformationMethod; // 引入显示密码的类
import android.text.method.PasswordTransformationMethod; // 引入隐藏密码的类
import android.util.Log; // 引入日志类，用于输出日志
import android.view.View; // 引入视图类
import android.view.Window; // 引入窗口类
import android.view.WindowManager; // 引入窗口管理类
import android.widget.Button; // 引入按钮类
import android.widget.EditText; // 引入编辑文本类
import android.widget.ImageView; // 引入图像视图类
import android.widget.LinearLayout; // 引入线性布局类
import android.widget.TextView; // 引入文本视图类

import androidx.annotation.NonNull; // 引入非空注解
import androidx.annotation.RequiresApi; // 引入需要API注解
import androidx.appcompat.app.AppCompatActivity; // 引入兼容性活动类
import androidx.core.app.ActivityCompat; // 引入活动权限兼容类
import androidx.core.view.ViewCompat; // 引入视图兼容类
import androidx.core.view.WindowInsetsControllerCompat; // 引入窗口插入控制类

import org.greenrobot.eventbus.EventBus; // 引入EventBus库，用于事件通信
import org.greenrobot.eventbus.Subscribe; // 引入EventBus订阅注解
import org.greenrobot.eventbus.ThreadMode; // 引入线程模式

import java.util.Objects; // 引入工具类，用于比较对象

import car.bkrc.com.car2024.MessageBean.DataRefreshBean; // 引入数据刷新类
import car.bkrc.com.car2024.R; // 引入资源文件
import car.bkrc.com.car2024.Utils.CameraUtils.CameraSearchService; // 引入摄像头搜索服务类
import car.bkrc.com.car2024.Utils.CameraUtils.XcApplication; // 引入应用全局类
import car.bkrc.com.car2024.Utils.OtherUtil.CameraConnectUtil; // 引入摄像头连接工具类
import car.bkrc.com.car2024.Utils.OtherUtil.ToastUtil; // 引入Toast工具类
import car.bkrc.com.car2024.Utils.OtherUtil.WiFiStateUtil; // 引入Wi-Fi状态工具类
import car.bkrc.com.car2024.Utils.dialog.DialogUtils; // 引入对话框工具类
import car.bkrc.com.car2024.Utils.dialog.ShowDialog; // 引入显示对话框类


public class LoginActivity extends AppCompatActivity implements View.OnClickListener {
    private EditText device_edit = null; // 设备ID输入框
    private EditText login_edit = null; // 登录名输入框
    private EditText passwd_edit = null; // 密码输入框
    private LinearLayout wifi_back, uart_back; // Wi-Fi和串口切换按钮的布局

    private Button bt_connect = null; // 连接按钮
    private ImageView rememberbox = null, uart_state_image, wifi_state_image; // 记住密码框，串口和Wi-Fi状态图标
    private TextView wifi_box = null, uart_box = null; // Wi-Fi和串口状态文本
    private boolean passwordState = false; // 密码显示状态
    private ProgressDialog dialog = null; // 进度对话框

    /**
     * 权限申请结果返回
     */
    void Request() {
        if (checkSelfPermission(Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // 如果没有Wi-Fi定位权限，请求权限
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, 1);
        } else {
            Log.e("WIFISSID", getConnectWifiSsid()); // 输出当前Wi-Fi的SSID
            if ("<unknown ssid>".equals(getConnectWifiSsid())) { // 如果Wi-Fi未连接
                ToastUtil.ShowToast(this, "当前未接入到Wi-Fi网络中，请连接平台Wi-Fi");
            } else if (getConnectWifiSsid().contains("BKRC")) { // 如果Wi-Fi连接的是指定的网络
                ToastUtil.ShowToast(this, "当前连接WiFi：" + getConnectWifiSsid().replaceAll("\"", ""));
            } else { // 如果Wi-Fi连接的是不正确的网络
                ToastUtil.ShowToast(this, "请检查WiFi是否连接正确！");
            }
            if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                // 如果没有存储读取权限，请求权限
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE
                }, 3);
            }
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                // 如果是Android 11及以上版本，申请特殊权限
                Request30();
            }
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.R)
    void Request30() {
        if (checkSelfPermission(Manifest.permission.MANAGE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            // 如果没有管理外部存储权限，请求权限
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.MANAGE_EXTERNAL_STORAGE}, 4);
        }
    }

    /**
     * 判断是否首次启动
     */
    private boolean firstRun() {
        SharedPreferences sharedPreferences = getSharedPreferences("FirstRun", 0);
        // 获取共享首选项，检查是否是首次运行
        String first_run = sharedPreferences.getString("First", "2024-V1.0");
        if (first_run.equals("2024-V1.1")) {
            return false;
        } else {
            sharedPreferences.edit().putString("First", "2024-V1.1").apply();
            return true;
        }
    }

    /**
     * 应用升级提醒弹窗
     */
    private void upDialog() {
        ShowDialog showDialog = new ShowDialog();
        // 创建并显示升级提醒对话框
        showDialog.show(LoginActivity.this, "应用更新说明");
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == 1) {
            if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                // 如果Wi-Fi定位权限请求成功
                if (Objects.equals(getConnectWifiSsid(), "<unknown ssid>")) {
                    ToastUtil.ShowToast(this, "当前未接入到Wi-Fi网络中，请连接平台Wi-Fi");
                } else if (getConnectWifiSsid().contains("BKRC")) {
                    ToastUtil.ShowToast(this, "当前连接WiFi：" + getConnectWifiSsid().replaceAll("\"", ""));
                } else {
                    ToastUtil.ShowToast(this, "请检查WiFi是否连接正确！");
                }
            }
        }
    }

    /**
     * Activity创建时调用
     * @param savedInstanceState 如果活动之前被销毁，包含恢复数据，否则为null
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);
        // 设置无标题栏
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN); // 设置全屏
        Window window = getWindow();
        window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
        // 设置系统栏背景
        window.addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS);
        // 设置布局不受系统栏限制
        WindowInsetsControllerCompat wic = ViewCompat.getWindowInsetsController(getWindow().getDecorView());
        if (wic != null) {
            wic.setAppearanceLightStatusBars(true); // 设置状态栏字体为黑色
        }

        setContentView(R.layout.activity_login); // 设置活动布局文件

        EventBus.getDefault().register(this); // 注册EventBus消息订阅
        CameraConnectUtil cameraConnectUtil = new CameraConnectUtil(this);
        // 创建摄像头连接工具类
        findViews();  // 初始化控件
        cameraConnectUtil.cameraInit(); // 初始化摄像头
        Request();
        // 请求权限
        if (firstRun()) {
            upDialog();
            // 如果是首次启动，显示更新提示
        }
    }
    /**
     * 判断当前设备是手机还是平板，代码来自 Google I/O App for Android
     * @return 平板返回 True，手机返回 False
     */
    public static boolean isPad(Context context) {
        return (context.getResources().getConfiguration().screenLayout
                & Configuration.SCREENLAYOUT_SIZE_MASK)
                >= Configuration.SCREENLAYOUT_SIZE_LARGE; // 判断屏幕尺寸，判断设备是否为平板
    }

    @Override
    protected void onResume() {
        super.onResume();
        // 活动重新获得焦点时调用
    }

    /**
     * 初始化页面控件
     */
    @SuppressLint({"UseCompatLoadingForDrawables", "SetTextI18n"})
    private void findViews() {
        device_edit = findViewById(R.id.deviceid); // 获取设备ID输入框
        login_edit = findViewById(R.id.loginname); // 获取登录名输入框
        passwd_edit = findViewById(R.id.loginpasswd); // 获取密码输入框
        Button bt_reset = findViewById(R.id.reset); // 获取重置按钮
        bt_connect = findViewById(R.id.connect); // 获取连接按钮
        rememberbox = findViewById(R.id.remember); // 获取记住密码框
        wifi_state_image = findViewById(R.id.wifi_image); // 获取Wi-Fi状态图标
        uart_state_image = findViewById(R.id.uart_image); // 获取串口状态图标
        wifi_box = findViewById(R.id.wifi_each); // 获取Wi-Fi状态文本
        uart_box = findViewById(R.id.uart_each); // 获取串口状态文本
        wifi_back = findViewById(R.id.wifi_back); // 获取Wi-Fi切换按钮布局
        uart_back = findViewById(R.id.uart_back); // 获取串口切换按钮布局
        if (cameraDataCheck(CAMERAUSER).equals("")) {
            login_edit.setText("admin"); // 如果没有保存摄像头用户名，设置为默认用户名
            passwd_edit.setText("888888"); // 如果没有保存摄像头密码，设置为默认密码
        } else {
            login_edit.setText("admin"); // 如果保存了用户名，设置为默认用户名
            passwd_edit.setText(cameraDataCheck(CAMERAPS)); // 设置保存的密码
        }
        bt_reset.setOnClickListener(this); // 设置重置按钮点击监听
        bt_connect.setOnClickListener(this); // 设置连接按钮点击监听
        rememberbox.setOnClickListener(v -> {
            vSimple(getApplicationContext(), 30); // 控制手机震动进行反馈
            setPasswordState(!passwordState); // 切换密码显示状态
        });

        wifi_back.setOnClickListener(v -> {
            vSimple(getApplicationContext(), 30); // 控制手机震动进行反馈
            wifi_box.setTextColor(getResources().getColor(R.color.switch_black)); // 设置Wi-Fi文本为黑色
            uart_box.setTextColor(getResources().getColor(R.color.shift_color_gray)); // 设置串口文本为灰色
            uart_state_image.setBackground(getResources().getDrawable(R.drawable.ic_uart_off)); // 设置串口状态图标为关闭
            wifi_state_image.setBackground(getResources().getDrawable(R.drawable.ic_wifi)); // 设置Wi-Fi状态图标为打开
            wifi_back.setBackground(getResources().getDrawable(R.drawable.login_switch_background_on)); // 设置Wi-Fi按钮背景为打开
            uart_back.setBackground(getResources().getDrawable(R.drawable.login_switch_background_off)); // 设置串口按钮背景为关闭
            XcApplication.isserial = XcApplication.Mode.SOCKET; // 设置为Wi-Fi模式
        });
        uart_back.setOnClickListener(v -> {
            vSimple(getApplicationContext(), 30); // 控制手机震动进行反馈
            uart_box.setTextColor(getResources().getColor(R.color.switch_black)); // 设置串口文本为黑色
            wifi_box.setTextColor(getResources().getColor(R.color.shift_color_gray)); // 设置Wi-Fi文本为灰色
            uart_state_image.setBackground(getResources().getDrawable(R.drawable.ic_uart)); // 设置串口状态图标为打开
            wifi_state_image.setBackground(getResources().getDrawable(R.drawable.ic_wifi_off)); // 设置Wi-Fi状态图标为关闭
            wifi_back.setBackground(getResources().getDrawable(R.drawable.login_switch_background_off)); // 设置Wi-Fi按钮背景为关闭
            uart_back.setBackground(getResources().getDrawable(R.drawable.login_switch_background_on)); // 设置串口按钮背景为打开
            XcApplication.isserial = XcApplication.Mode.USB_SERIAL; // 设置为串口模式
            if (cameraDataCheck(CAMERAIP).equals("")) {
                ToastUtil.ShowToast(this, "请设置CameraIP"); // 提示用户设置CameraIP
                DialogUtils.showCompleteDialog(this, "设置CameraIP", cameraDataCheck(CAMERAIP)); // 显示CameraIP设置对话框
            }
        });
        uart_back.setOnLongClickListener(v -> {
            vSimple(getApplicationContext(), 30); // 控制手机震动进行反馈
            DialogUtils.showCompleteDialog(LoginActivity.this, "设置CameraIP", cameraDataCheck(CAMERAIP));
            // 长按串口切换按钮显示CameraIP设置对话框
            return false;
        });
    }

    private final String CAMERAIP = "CameraIP"; // 摄像头IP存储的key
    private final String CAMERAUSER = "CameraUser"; // 摄像头用户名存储的key
    private final String CAMERAPS = "CameraPs"; // 摄像头密码存储的key

    /**
     * 判断本地保存摄像头的相关信息
     *
     * @param key CameraIP:查询本地是否保存了串口通信模式的IP地址
     *            CameraUser：查询是否保存了摄像头访问账户
     *            CameraPS：查询是否保存了摄像头访问账户的密码
     */
    private String cameraDataCheck(String key) {
        SharedPreferences sharedPreferences = getSharedPreferences("IPCheck", 0);
        String first_run = sharedPreferences.getString(key, ""); // 获取存储的数据
        if (first_run.equals("")) {
            return ""; // 如果没有数据返回空字符串
        } else {
            return first_run; // 返回保存的数据
        }
    }

    /**
     * 保存指定数据到本地
     */
    private void rememberData(String key, String data) {
        SharedPreferences sharedPreferences = getSharedPreferences("IPCheck", 0); // 获取共享首选项
        sharedPreferences.edit().putString(key, data).apply(); // 保存数据
    }

    /**
     * 设置密码隐藏/显示状态
     *
     * @param state state = true : 显示
     *              state = false ： 隐藏
     */
    @SuppressLint("UseCompatLoadingForDrawables")
    private void setPasswordState(boolean state) {
        if (state) {
            passwd_edit.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
            // 如果状态为显示，设置密码框为明文显示
            rememberbox.setBackground(getResources().getDrawable(R.drawable.ic_on));
            // 设置记住密码框背景为开启
            passwordState = true; // 设置密码显示状态为显示
        } else {
            passwd_edit.setTransformationMethod(PasswordTransformationMethod.getInstance());
            // 如果状态为隐藏，设置密码框为隐藏密码
            rememberbox.setBackground(getResources().getDrawable(R.drawable.ic_off));
            // 设置记住密码框背景为关闭
            passwordState = false; // 设置密码显示状态为隐藏
        }
    }

    /**
     * 获取WiFi名称信息
     * @return 返回WiFi名称，字符串形式
     */
    private String getConnectWifiSsid() {
        WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(WIFI_SERVICE);
        // 获取Wi-Fi管理器
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();
        // 获取当前Wi-Fi连接信息
        Log.d("wifiInfo", wifiInfo.toString()); // 输出Wi-Fi信息
        Log.d("SSID", wifiInfo.getSSID()); // 输出Wi-Fi的SSID
        return wifiInfo.getSSID(); // 返回当前Wi-Fi的SSID
    }

    /**
     * 控件点击监听
     * @param view The view that was clicked.
     */
    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.reset) {
            vSimple(getApplicationContext(), 30); // 控制手机震动进行反馈
            device_edit.setText(""); // 清空设备ID输入框
            setPasswordState(false); // 设置密码为隐藏状态
        } else if (view.equals(bt_connect)) {
            vSimple(getApplicationContext(), 30); // 控制手机震动进行反馈
            if ((XcApplication.isserial == XcApplication.Mode.USB_SERIAL) && cameraDataCheck(CAMERAIP).equals("")) {
                // 如果是串口模式，且没有设置CameraIP
                ToastUtil.ShowToast(this, "请设置CameraIP");
                DialogUtils.showCompleteDialog(this, "设置CameraIP", cameraDataCheck(CAMERAIP));
                // 提示用户设置CameraIP并显示设置对话框
            } else {
                if (passwd_edit.getText().toString().equals("")) {
                    ToastUtil.ShowToast(this, "请输入账户密码（摄像头密码，默认为：888888）！");
                    // 提示用户输入密码
                } else {
                    new Thread(() -> {
                        rememberData(CAMERAUSER, login_edit.getText().toString());
                        rememberData(CAMERAPS, passwd_edit.getText().toString());
                        // 保存摄像头用户名和密码
                    }).start();
                    dialog = new ProgressDialog(this);
                    dialog.setMessage("撸起袖子加载中...");
                    dialog.show(); // 显示加载对话框
                    if (XcApplication.isserial == XcApplication.Mode.SOCKET) {
                        useNetwork();
                        // 使用网络模式
                    } else {
                        useUart();
                        // 使用串口模式
                    }
                }
            }
        }
    }

    /**
     * 接收Eventbus消息
     */
    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onEventMainThread(DataRefreshBean refresh) {
        if (refresh.getRefreshState() == 2) {
            startFirstActivity();
            // 当接收到EventBus消息且刷新状态为2时，启动首页活动
        }
    }

    // 搜索摄像头cameraIP
    private void search() {
        Intent intent = new Intent(LoginActivity.this, CameraSearchService.class);
        // 创建启动摄像头搜索服务的Intent
        startService(intent);
        // 启动服务
    }

    private void useUart() {
        // 搜索摄像头然后启动摄像头
        search();
    }

    private void useNetwork() {
        //2. 网络模式下，初始化WiFi
        if (new WiFiStateUtil(this).wifiInit()) {
            // 如果Wi-Fi初始化成功
            search();
            // 搜索摄像头
        } else {
            dialog.cancel(); // 取消进度对话框
            ToastUtil.ShowToast(this, "平台未连接，正在跳过...");
            // 提示用户平台未连接
            new WiFiStateUtil(this).openWifi();
            // 打开Wi-Fi
            search();
        }
    }

    /**
     * 切换到首页
     */
    private void startFirstActivity() {
        dialog.cancel(); // 关闭进度对话框
        startActivity(new Intent(LoginActivity.this, FirstActivity.class));
        // 启动首页活动
        finish(); // 结束当前登录活动
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // Activity销毁时调用
        EventBus.getDefault().unregister(this); // 注销EventBus
        if (dialog != null) {
            dialog.cancel(); // 取消对话框
        }
        Log.e("LoginActivity", "onDestroy");
        // 输出日志
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        // Activity重新启动时调用
        Log.e("LoginActivity", "onRestart");
        // 输出日志
    }

    /**
     * 控制手机震动
     *
     * @param context     上下文
     * @param millisecond 震动时间，毫秒为单位
     */
    public static void vSimple(Context context, int millisecond) {
        Vibrator vibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
        // 获取震动服务
        vibrator.vibrate(millisecond);
        // 震动指定时间
    }
}
