package com.example.sms_control;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.telephony.gsm.SmsManager;
import android.telephony.SmsMessage;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.Toast;




public class MainActivity extends AppCompatActivity {
    //Tạo một đối tượng BroadcastReceiver
    BroadcastReceiver receiver=null;
    //Khai báo các biến
    Button btn_on1;
    Button btn_off1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//ẩn thanh tiêu đề của ứng dụng
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);
//ánh xạ các biến và các UI control
        btn_on1 = (Button) findViewById(R.id.btn_on);
        btn_off1 = (Button) findViewById(R.id.btn_off);
    //viết hàm xử lý khi nút btn_on1 được nhấn
        btn_on1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendSMS("+84948161610", "SANG");
            }
        });
//viết hàm xử lý nút btn_off1
        btn_off1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendSMS("+84948161610", "TAT");
            }
        });
//phần code xử lý tin nhắn phản hồi từ module sim
//tạo bộ lắng nghe để nhận tin nhắn đến
        IntentFilter filter_sms= new  IntentFilter("android.provider.Telephony.SMS_RECEIVED");
        receiver=new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
//hàm này sẽ thực thi khi tin nhắn được gởi đến
//khai báo chuỗi có nội dung "pdus" để nhận tin nhắn
                String sms_extra="pdus";
//khai báo đóng gói bundle để nhận gói dữ liệu
                Bundle bundle=intent.getExtras();
//đóng gói bundle sẽ trả về tập các tin nhắn gửi đến cùng một lúc
                Object[] smsArr=(Object[])bundle.get(sms_extra);
                String body="";
                String address="";
//Dùng vòng lặp để đọc từng tin nhắn
                for(int i=0;i<smsArr.length;i++)
                {
                    SmsMessage smsMsg =  SmsMessage.createFromPdu((byte[]) smsArr[i]);
//lấy nội dung tin nhan
                    body = smsMsg.getMessageBody();
//lấy địa chỉ (so dien thoai) của thiết bị gởi tin nhắn
                    address = smsMsg.getDisplayOriginatingAddress();
                }

                if(address.equals("+84948161610"))

                {
                    if(body.equals("LED on"))
                    {

                        Object on;
                        Toast.makeText(context,"LED on",Toast.LENGTH_LONG ).show();
                    }
                    else if(body.equals("LED off"))
                    {
                        Toast.makeText(context,"LED off",
                                Toast.LENGTH_LONG).show();
                    }
                    }
            }
        };
//đăng ký bộ lắng nghe vào hệ thống
        registerReceiver(receiver,filter_sms);
    }
    //Hàm xử lý gởi tin nhắn
    public void sendSMS(String phonenumber, String message){
        SmsManager sms= SmsManager.getDefault();
        sms.sendTextMessage(phonenumber,null,message,null,null);
    }
    //huy bo dang ky lang nghe nhan tin nhan khi tat ung dung
    protected void onDestroy(){
        super.onDestroy();
        unregisterReceiver(receiver);
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
// Inflate the menu; this adds items to the action bar if
       // it is present.
                getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
// Handle action bar item clicks here. The action bar will
// automatically handle clicks on the Home/Up
        //button, so long
        // as you specify a parent activity in
       // AndroidManifest.xml.
        int id = item.getItemId();
//noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}