package com.example.myapplication;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.nfc.Tag;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.bottomnavigation.BottomNavigationView;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttClientPersistence;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
import org.json.JSONException;
import org.json.JSONObject;

import java.math.BigInteger;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.HashMap;
import java.util.Map;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;

public class MainActivity extends AppCompatActivity {
    private static final String TAG =MainActivity.class.getSimpleName();
    private String productKey="k0tcvSoOQiQ";
    private String deviceName="app_to_stm";
    private String deviceSecret="8cae02658a063f89ce6f863e9959bbdb";
    private MqttClient mqttClient=null;
    private final String led_light_on="{\"led_light\":1}";
    private final String led_light_off="{\"led_light\":0}";
    private final String fan_temp_on="{\"fan_temp\":1}";
    private final String fan_temp_off="{\"fan_temp\":0}";
    private final String fan_humid_on="{\"fan_humid\":1}";
    private final String fan_humid_off="{\"fan_humid\":0}";
    private final String humid_add_on="{\"humid_add\":1}";
    private final String humid_add_off="{\"humid_add\":0}";
    private final String water_pump_on="{\"water_pump\":1}";
    private final String water_pump_off="{\"water_pump\":0}";
    private final String control_mode_on="{\"control_mode\":1}";
    private final String control_mode_off="{\"control_mode\":0}";
    final private String PUB_TOPIC = "/" + productKey + "/" + deviceName + "/user/update";
    final private String SUB_TOPIC = "/" + productKey + "/" + deviceName + "/user/get";

    private String responseBody = "";

    private String host = "iot-06z00abrroxfvgl.mqtt.iothub.aliyuncs.com";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initAliyunIoTClient();
        TextView textView_1 =findViewById(R.id.textView36);//散热
        TextView textView_2 =findViewById(R.id.textView39);//除湿
        TextView textView_3 =findViewById(R.id.textView41);//遮光
        TextView textView_4 =findViewById(R.id.textView43);//补光
        TextView textView_5 =findViewById(R.id.textView44);//水泵
        TextView textView_6 =findViewById(R.id.textView47);//control
        @SuppressLint({"MissingInflatedId", "LocalSuppress", "UseSwitchCompatOrMaterialCode"}) Switch sw = findViewById(R.id.switch2);
        sw.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                String date;
                if (isChecked){
                    date="水泵已打开";
                    try {
                        publishMessage(water_pump_on);
                        textView_5.setText("开");
                    } catch (MqttException e) {
                        throw new RuntimeException(e);
                    }

                }
                else{
                    date="水泵已关闭";
                    try {
                        publishMessage(water_pump_off);
                        textView_5.setText("关");
                    } catch (MqttException e) {
                        throw new RuntimeException(e);
                    }
                }
                Toast.makeText(MainActivity.this,date,Toast.LENGTH_SHORT).show();
            }
        });

        @SuppressLint({"MissingInflatedId", "LocalSuppress", "UseSwitchCompatOrMaterialCode"}) Switch sw1 = findViewById(R.id.switch3);
        sw1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                String date;
                if (isChecked){
                    date="补光灯已打开";
                    try {
                        publishMessage(led_light_on);
                        textView_4.setText("开");
                    } catch (MqttException e) {
                        throw new RuntimeException(e);
                    }
                }
                else{
                    date="补光灯已关闭";
                    try {
                        publishMessage(led_light_off);
                        textView_4.setText("关");
                    } catch (MqttException e) {
                        throw new RuntimeException(e);
                    }
                }
                Toast.makeText(MainActivity.this,date,Toast.LENGTH_SHORT).show();
            }
        });

        @SuppressLint({"MissingInflatedId", "LocalSuppress", "UseSwitchCompatOrMaterialCode"}) Switch sw2 = findViewById(R.id.switch6);
        sw2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                String date;
                if (isChecked){
                    date="散热器已打开";
                    try {
                        publishMessage(fan_temp_on);
                        textView_1.setText("开");
                    } catch (MqttException e) {
                        throw new RuntimeException(e);
                    }
                }
                else{
                    date="散热器已关闭";
                    try {
                        publishMessage(fan_temp_off);
                        textView_1.setText("关");
                    } catch (MqttException e) {
                        throw new RuntimeException(e);
                    }
                }
                Toast.makeText(MainActivity.this,date,Toast.LENGTH_SHORT).show();
            }
        });

        @SuppressLint({"MissingInflatedId", "LocalSuppress", "UseSwitchCompatOrMaterialCode"}) Switch sw3 = findViewById(R.id.switch5);
        sw3.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                String date;
                if (isChecked){
                    date="暖光灯已打开";
                    try {
                        publishMessage(fan_humid_on);
                        textView_2.setText("开");
                    } catch (MqttException e) {
                        throw new RuntimeException(e);
                    }
                }
                else{
                    date="暖光灯已关闭";
                    try {
                        publishMessage(fan_humid_off);
                        textView_2.setText("关");
                    } catch (MqttException e) {
                        throw new RuntimeException(e);
                    }
                }
                Toast.makeText(MainActivity.this,date,Toast.LENGTH_SHORT).show();
            }
        });

        @SuppressLint({"MissingInflatedId", "LocalSuppress", "UseSwitchCompatOrMaterialCode"}) Switch sw4 = findViewById(R.id.switch4);
        sw4.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                String date;
                if (isChecked){
                    date="遮光棚已打开";
                    try {
                        publishMessage(humid_add_on);
                        textView_3.setText("开");
                    } catch (MqttException e) {
                        throw new RuntimeException(e);
                    }
                }
                else{
                    date="遮光棚已关闭";
                    try {
                        publishMessage(humid_add_off);
                        textView_3.setText("关");
                    } catch (MqttException e) {
                        throw new RuntimeException(e);
                    }
                }
                Toast.makeText(MainActivity.this,date,Toast.LENGTH_SHORT).show();
            }
        });

        @SuppressLint({"MissingInflatedId", "LocalSuppress", "UseSwitchCompatOrMaterialCode"}) Switch sw5 = findViewById(R.id.switch1);
        sw5.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                String date;
                if (isChecked){
                    date="自动模式";
                    try {
                        publishMessage(control_mode_on);
                        textView_6.setText("自动");
                    } catch (MqttException e) {
                        throw new RuntimeException(e);
                    }
                }
                else{
                    date="手动模式";
                    try {
                        publishMessage(control_mode_off);
                        textView_6.setText("手动");
                    } catch (MqttException e) {
                        throw new RuntimeException(e);
                    }
                }
                Toast.makeText(MainActivity.this,date,Toast.LENGTH_SHORT).show();
            }
        });

    }
    private void initAliyunIoTClient() {

        try {
            String clientId = "12345"+ System.currentTimeMillis();

            Map<String, String> params = new HashMap<String, String>(16);
            params.put("productKey", productKey);
            params.put("deviceName", deviceName);
            params.put("clientId", clientId);
            String timestamp = String.valueOf(System.currentTimeMillis());
            params.put("timestamp", timestamp);

            // cn-shanghai
            String targetServer ="tcp://"+ productKey + ".iot-as-mqtt.cn-shanghai.aliyuncs.com:1883";

            String mqttclientId = clientId + "|securemode=3,signmethod=hmacsha1,timestamp=" + timestamp + "|";
            String mqttUsername = deviceName + "&" + productKey;
            String mqttPassword = AliyunIoTSignUtil.sign(params, deviceSecret, "hmacsha1");

            connectMqtt(targetServer, mqttclientId, mqttUsername, mqttPassword);


        } catch (Exception e) {
            e.printStackTrace();
            responseBody = e.getMessage();
        }

    }
    //订阅主题
    public void subscribeTopic(String topic) throws MqttException {
        mqttClient.subscribe(topic);
    }
    public void connectMqtt(String url, String clientId, String mqttUsername, String mqttPassword) throws Exception {

        MemoryPersistence persistence = new MemoryPersistence();
        mqttClient = new MqttClient(url, clientId, persistence);
        MqttConnectOptions connOpts = new MqttConnectOptions();
        // MQTT 3.1.1
        connOpts.setMqttVersion(4);
        connOpts.setAutomaticReconnect(true);
        connOpts.setCleanSession(true);

        connOpts.setUserName(mqttUsername);
        connOpts.setPassword(mqttPassword.toCharArray());
        connOpts.setKeepAliveInterval(60);

        mqttClient.connect(connOpts);
        subscribeTopic(SUB_TOPIC);
        Log.d(TAG, "connected " + url);

        mqttClient.setCallback(new MqttCallback() {
            @Override
            public void connectionLost(Throwable throwable) {
                Log.i(TAG, "connection lost");
            }

            @Override
            public void messageArrived(String s, MqttMessage mqttMessage) throws Exception {
                String payload = new String(mqttMessage.getPayload());
                TextView wendu = findViewById(R.id.textView12);
                TextView shidu = findViewById(R.id.textView17);
                TextView ph = findViewById(R.id.textView9);
                TextView ec = findViewById(R.id.textView10);
                TextView CO2 = findViewById(R.id.textView23);
                TextView light = findViewById(R.id.textView25);
                TextView soil_wendu = findViewById(R.id.textView28);
                TextView soil_shidu = findViewById(R.id.textView32);
                wendu.setText(jsonParser_temp(payload));
                shidu.setText(jsonParser_humd(payload));
                ph.setText(jsonParser_PH(payload));
                ec.setText(jsonParser_EC(payload));
                CO2.setText(jsonParser_CO2(payload));
                light.setText(jsonParser_light(payload));
                soil_shidu.setText(jsonParser_soilHumd(payload));
                soil_wendu.setText(jsonParser_soilTemp(payload));



            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
                Log.i(TAG, "msg delivered");
            }
        });

    }
    public void publishMessage(String payload) throws MqttException {
        MqttMessage message = new MqttMessage();
        message.setPayload(payload.getBytes());
        message.setQos(0);
        mqttClient.publish(PUB_TOPIC,message);
    }

    private String jsonParser_temp(String strIn) throws JSONException {
        String num = "1";
        JSONObject paramsObject = null;
        try {
            JSONObject jsonObject = new JSONObject(strIn);
            paramsObject=jsonObject.getJSONObject("params");
            num=paramsObject.getString("temperature");
        }catch (JSONException e){
            e.printStackTrace();
        }
        return num;
    }

    private String jsonParser_humd(String strIn) throws JSONException {
        String num = "1";
        JSONObject paramsObject = null;
        try {
            JSONObject jsonObject = new JSONObject(strIn);
            paramsObject=jsonObject.getJSONObject("params");
            num=paramsObject.getString("Humidity");
        }catch (JSONException e){
            e.printStackTrace();
        }
        return num;
    }

    private String jsonParser_PH(String strIn) throws JSONException {
        String num = "1";
        JSONObject paramsObject = null;
        try {
            JSONObject jsonObject = new JSONObject(strIn);
            paramsObject=jsonObject.getJSONObject("params");
            num=paramsObject.getString("soilPH");
        }catch (JSONException e){
            e.printStackTrace();
        }
        return num;
    }

    private String jsonParser_EC(String strIn) throws JSONException {
        String num = "1";
        JSONObject paramsObject = null;
        try {
            JSONObject jsonObject = new JSONObject(strIn);
            paramsObject=jsonObject.getJSONObject("params");
            num=paramsObject.getString("conductivity");
        }catch (JSONException e){
            e.printStackTrace();
        }
        return num;
    }

    private String jsonParser_CO2(String strIn) throws JSONException {
        String num = "1";
        JSONObject paramsObject = null;
        try {
            JSONObject jsonObject = new JSONObject(strIn);
            paramsObject=jsonObject.getJSONObject("params");
            num=paramsObject.getString("CO2");
        }catch (JSONException e){
            e.printStackTrace();
        }
        return num;
    }

    private String jsonParser_light(String strIn) throws JSONException {
        String num = "1";
        JSONObject paramsObject = null;
        try {
            JSONObject jsonObject = new JSONObject(strIn);
            paramsObject=jsonObject.getJSONObject("params");
            num=paramsObject.getString("LightLux");
        }catch (JSONException e){
            e.printStackTrace();
        }
        return num;
    }

    private String jsonParser_soilTemp(String strIn) throws JSONException {
        String num = "1";
        JSONObject paramsObject = null;
        try {
            JSONObject jsonObject = new JSONObject(strIn);
            paramsObject=jsonObject.getJSONObject("params");
            num=paramsObject.getString("soilTemperature");
        }catch (JSONException e){
            e.printStackTrace();
        }
        return num;
    }

    private String jsonParser_soilHumd(String strIn) throws JSONException {
        String num = "1";
        JSONObject paramsObject = null;
        try {
            JSONObject jsonObject = new JSONObject(strIn);
            paramsObject=jsonObject.getJSONObject("params");
            num=paramsObject.getString("water");
        }catch (JSONException e){
            e.printStackTrace();
        }
        return num;
    }

}