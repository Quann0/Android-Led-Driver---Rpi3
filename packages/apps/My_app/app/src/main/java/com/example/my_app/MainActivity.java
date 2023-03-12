package com.example.my_app;


import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.app.Activity;
import android.view.View;
import android.view.View.OnClickListener;
import android.os.IOpersysService;
import android.os.ServiceManager;

public class MainActivity extends Activity {
    private final static String LOG_TAG = "PhuLA HelloOpersysInternal";

    private IOpersysService blinkService = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button my_button = (Button)findViewById(R.id.button_led_on);
        Button my_button1 = (Button)findViewById(R.id.button_led_off);
        TextView my_textview = (TextView)findViewById(R.id.my_text);

        blinkService = IOpersysService.Stub.asInterface(ServiceManager.getService("opersys"));

        Log.i(LOG_TAG, "Blink Activity Created");
        blinkService.write("7");
        blinkService.write("1");
        my_button.setOnClickListener(new OnClickListener() {
             @Override
             public void onClick(View view) {
                 my_textview.setText("LedOn Called!");
                 try {
                     String val = "1";
                     blinkService.write(val);
                 } catch (RemoteException e) {
                     Log.e(LOG_TAG, "Remote Exception while writing value to device.");
                 }
             }
         });
        my_button1.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                my_textview.setText("LedOff Called!");
                try {
                    String val = "0";
                    blinkService.write(val);
                } catch (RemoteException e) {
                    Log.e(LOG_TAG, "Remote Exception while writing value to device.");
                }
            }
        });
        //my_button.callOnClick();
    }
}