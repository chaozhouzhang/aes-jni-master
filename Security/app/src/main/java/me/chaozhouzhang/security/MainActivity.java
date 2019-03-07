package me.chaozhouzhang.security;

import android.os.Bundle;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

/**
 * @author zhangchaozhou
 */
public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        String en = AESEncrypt.encode(this, "明文内容");
        String de = AESEncrypt.decode(this, en);
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(en + "\n" + de);

    }


}
