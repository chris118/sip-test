//
//  siputil.hpp
//  sip
//
//  Created by Chris on 2018/7/12.
//  Copyright © 2018年 Chris. All rights reserved.
//

#ifndef siputil_hpp
#define siputil_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
// #include "hhsolarctrl.h"
// #include "solarexception.h"
// using namespace HHTx2;
#include <glog/logging.h>
using namespace std;

#define random(a, b) (rand() % (b - a + 1) + a)

class SIPUtil
{
public:
    SIPUtil() {}

    string getCapacityVoltage()
    {
        // auto dev = shared_ptr<HHsolarctrlImpl>(new HHsolarctrlImpl());
        // float percent_voltage = dev->getBatteryLevel();
        // int capacity = percent_voltage * 200;
        // stringstream ss;
        // ss << capacity;
        // string capacity_s = ss.str();

        return "12.0";
    }

    string change_control_camera_ip(char *cSip, int cSip_length, string platform_ip, string ip_port_platform, string ip_port_camera)
    {
        vector<string> lines;
        SplitString(string(cSip), lines, "\r\n");

        // line 1
        string line1 = lines[0];
        string line1_result = change_ip(line1, "@", " SIP/2.0", ip_port_camera);

        // line2
        string line2 = lines[1];
        string line2_result = "";
        if (line2.find(";branch") > 0)
        {
            line2_result = change_ip(line2, "UDP ", ";branch", ip_port_platform, 3);
        }
        else
        {
            line2_result = change_ip(line2, "UDP ", ";rport", ip_port_platform, 3);
        }

        // line4
        string line4 = lines[3];
        string line4_result = change_ip(line4, "@", ">", ip_port_camera);

        // line5
        string line5 = lines[4];
        string line5_result = change_ip(line5, "@", ">;tag", ip_port_platform);

        // line6
        string line6 = lines[5];
        string line6_result = "";
        int index = line6.find("@");
        string s1 = line6.substr(0, index + 1);
        line6_result = s1 + platform_ip;

        string target = "";
        for (int i = 0; i < lines.size(); i++)
        {
            if (i == 0)
            {
                target = target + line1_result;
            }
            else if (i == 1)
            {
                target = target + line2_result;
            }
            else if (i == 3)
            {
                target = target + line4_result;
            }
            else if (i == 4)
            {
                target = target + line5_result;
            }
            else if (i == 5)
            {
                target = target + line6_result;
            }
            else
            {
                target = target + lines[i];
            }
            target = insertRN(target);
        }

        DLOG(INFO) << target << endl;
        //        printBuffer((char *)target.c_str(), target.length(), "");

        return target;

        // vector<string> lines;
        // SplitString(string(cSip), lines, "\r\n");

        // // line 1
        // string line1 = lines[0];
        // string line1_result = change_ip(line1, "@", " SIP/2.0", ip_port_camera);

        // // line2
        // string line2 = lines[1];
        // string line2_result = change_ip(line2, "UDP ", ";rport", ip_port_platform, 3);

        // // line3
        // string line3 = lines[2];
        // string line3_result = change_ip(line3, "@", ">;tag", ip_port_platform);

        // // line4
        // string line4 = lines[3];
        // string line4_result = change_ip(line4, "@", ">", ip_port_camera);

        // //        // line5
        // //        string line5 = lines[4];
        // //        string line5_result = "";
        // //        int index = line5.find("@");
        // //        string s1 = line5.substr(0, index + 1);
        // //        line5_result = s1 + platform_ip;

        // // line7
        // string line7 = lines[6];
        // string line7_result = change_ip(line7, "@", ">", ip_port_platform);

        // string target = "";
        // for (int i = 0; i < lines.size(); i++)
        // {
        //     //             if(i == 0) {
        //     //                 target = target + line1_result;
        //     //             }else if(i == 1){
        //     //                 target = target + line2_result;
        //     //             }else if(i == 2){
        //     //                 target = target + line3_result;
        //     //             }else if(i == 3){
        //     //                 target = target + line4_result;
        //     //             }
        //     // //            else if(i == 4){
        //     // //                target = target + line5_result;
        //     // //            }
        //     //             else if(i == 6){
        //     //                 target = target + line7_result;
        //     //             }else {
        //     //                 target = target + lines[i];
        //     //             }

        //     if (i == 6)
        //     {
        //         target = target + line7_result;
        //     }
        //     else
        //     {
        //         target = target + lines[i];
        //     }

        //     target = insertRN(target);
        //     // if(i != (lines.size() - 1)){
        //     //     target = insertRN(target); //不是最后一行
        //     // }
        // }

        // DLOG(INFO) << target << endl;
        // printBuffer((char *)target.c_str(), target.length(), "");

        // return target;
    }

    string get_voltage_message_sip(char *cSip, int cSip_length)
    {
        string capacity_s = getCapacityVoltage();

        vector<string> lines;
        SplitString(string(cSip), lines, "\r\n");

        //xml部分
        string codeline = lines[lines.size() - 1];
        vector<string> arr_xml;
        SplitString(codeline, arr_xml, "<Item Code=\"");
        string t = arr_xml[arr_xml.size() - 1];
        string code = t.substr(0, 18);

        string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
        xml = insertRN(xml);

        xml += "<SIP_XML EventType=\"Notify_ResidualPower\">";
        xml = insertRN(xml);

        xml += "<Item Code=\"" + code + "\" Capacity =\"" + capacity_s + "\" TotalCapacity =\"" + "200" + "\"/>";
        xml = insertRN(xml);

        xml += "</SIP_XML>";
        xml = insertRN(xml);

        //上部
        //        string line1 = lines[0];
        //        vector<string> arr_sip;
        //        SplitString(line1, arr_sip, "MESSAGE sip:");
        //
        //        string target = "NOTIFY sip:";
        //        target += arr_sip[1];
        //        target = insertRN(target);

        string target = "";
        target += lines[0];
        target = insertRN(target);

        target += lines[2];
        target = insertRN(target);

        target += lines[3];
        target = insertRN(target);

        target += lines[6];
        target = insertRN(target);

        target += lines[1];
        target = insertRN(target);

        target += lines[4];
        target = insertRN(target);

        target += lines[5];
        target = insertRN(target);

        /*     srand((unsigned)time(NULL));
        stringstream sSeq;
        sSeq << random(1,128);
        string seq_s = sSeq.str();
        
        target += "CSeq:" + seq_s + " MESSAGE";
        target = insertRN(target);*/

        target += lines[7];
        target = insertRN(target);

        stringstream stream;
        stream << xml.length();
        string new_length_str = stream.str();
        target += "Content-Length:   " + new_length_str;
        target = insertRN(target);
        target = insertRN(target);

        target += xml;

        DLOG(INFO) << target << endl;

        return target;
    }

    string get_subscrit_200_ok(char *cSip, int cSip_length)
    {
        vector<string> lines;
        SplitString(string(cSip), lines, "\r\n");

        string target = "SIP/2.0 200 OK";
        target = insertRN(target);
        target += lines[2];
        target = insertRN(target);
        target += lines[3];
        target = insertRN(target);
        target += lines[6];
        target = insertRN(target);
        target += lines[1];
        target = insertRN(target);
        target += lines[4];
        target = insertRN(target);
        target += lines[5];
        target = insertRN(target);
        target += "Content-Length: 0";
        target = insertRN(target);
        target = insertRN(target);

        return target;
    }

    string makeBatteryNotify(char *cSip, int cSip_length)
    {
        string capacity_s = getCapacityVoltage();

        vector<string> lines;
        SplitString(string(cSip), lines, "\r\n");

        //xml部分
        string codeline = lines[lines.size() - 2];
        vector<string> arr_xml;
        SplitString(codeline, arr_xml, "<Item Code=\"");
        string t = arr_xml[arr_xml.size() - 1];
        string code = t.substr(0, 18);

        string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
        xml = insertRN(xml);

        xml += "<SIP_XML EventType=\"Notify_ResidualPower\">"; //answer：Response_GetResidualPower
        xml = insertRN(xml);

        xml += "<Item Code=\"" + code + "\" Capacity =\"" + capacity_s + "\" TotalCapacity =\"" + "200" + "\"/>";
        xml = insertRN(xml);

        xml += "</SIP_XML>";
        xml = insertRN(xml);

        string target = "SIP/2.0 200 OK";
        target = insertRN(target);

        for (int i = 0; i <= 8; i++)
        {
            target += lines[i];
            target = insertRN(target);
        }

        //content-length
        stringstream stream;
        stream << xml.length();
        string new_length_str = stream.str();
        target += "Content-Length:   " + new_length_str;
        target = insertRN(target);

        //空行
        target += lines[10];
        target = insertRN(target);

        target += xml;

        DLOG(INFO) << target << endl;

        return target;
    }

    string get_rtsp_video_client_port(char *cSip, int cSip_length)
    {
        string c1_Start = "client_port=";
        char *cStart = (char *)c1_Start.c_str();
        int cStart_length = (int)c1_Start.length();
        int index_start = 0;
        bool matched = findIndex(cSip, cSip_length, cStart, cStart_length, index_start);
        if (matched)
        {
            index_start = index_start + cStart_length;
            //            DLOG(INFO) << "start: " << index_start << endl;
        }
        else
        {
            return NULL;
        }

        string cEnd_str = ";serve";
        int cEnd_length = (int)cEnd_str.length();
        char *cEnd = (char *)cEnd_str.c_str();

        int index_end = 0;
        matched = findIndex(cSip, cSip_length, cEnd, cEnd_length, index_end);
        if (matched)
        {
        }
        else
        {
            return NULL;
        }

        //原始ip
        int port_length = index_end - index_start;
        char cPort[port_length];
        memcpy(cPort, cSip + index_start, port_length);
        string strPort = string(cPort);
        strPort = strPort.substr(0, strPort.find("-"));

        DLOG(INFO) << "rtsp video client_port:" << strPort << endl;

        return strPort;
    }

    string get_rtsp_video_server_port(char *cSip, int cSip_length)
    {
        string c1_Start = "server_port=";
        char *cStart = (char *)c1_Start.c_str();
        int cStart_length = (int)c1_Start.length();
        int index_start = 0;
        bool matched = findIndex(cSip, cSip_length, cStart, cStart_length, index_start);
        if (matched)
        {
            index_start = index_start + cStart_length;
            //            DLOG(INFO) << "start: " << index_start << endl;
        }
        else
        {
            return "";
        }

        string cEnd_str = ";source=";
        int cEnd_length = (int)cEnd_str.length();
        char *cEnd = (char *)cEnd_str.c_str();

        int index_end = 0;
        matched = findIndex(cSip, cSip_length, cEnd, cEnd_length, index_end);
        if (matched)
        {
        }
        else
        {
            return "";
        }

        int port_length = index_end - index_start;
        char cPort[port_length];
        memcpy(cPort, cSip + index_start, port_length);
        string strPort = string(cPort);
        strPort = strPort.substr(0, strPort.find("-"));

        DLOG(INFO) << "rtsp video server_port:" << strPort << endl;

        return strPort;
    }

    shared_ptr<char> change_rtsp_ip_options(char *cSip, int cSip_length, string new_ip, int &new_len)
    {
        shared_ptr<char> newSip(new char[cSip_length]);
        newSip = change_ip_bewteen_start_end(cSip, cSip_length, "OPTIONS rtsp://", "/stategrid", new_ip, new_len);
        if (newSip == NULL)
        {
            newSip = change_ip_bewteen_start_end(cSip, cSip_length, "OPTIONS rtsp://", "/Streaming", new_ip, new_len);
        }
        return newSip;
    }

    shared_ptr<char> change_rtsp_ip_describe(char *cSip, int cSip_length, string new_ip, int &new_len)
    {
        shared_ptr<char> newSip(new char[cSip_length]);
        newSip = change_ip_bewteen_start_end(cSip, cSip_length, "DESCRIBE rtsp://", "/stategrid", new_ip, new_len);
        if (newSip == NULL)
        {
            newSip = change_ip_bewteen_start_end(cSip, cSip_length, "DESCRIBE rtsp://", "/Streaming", new_ip, new_len);
        }
        return newSip;
    }

    shared_ptr<char> change_rtsp_ip_setup(char *cSip, int cSip_length, string new_ip, int &new_len)
    {
        shared_ptr<char> newSip(new char[cSip_length]);
        newSip = change_ip_bewteen_start_end(cSip, cSip_length, "SETUP rtsp://", "/stategrid", new_ip, new_len);
        if (newSip == NULL)
        {
            newSip = change_ip_bewteen_start_end(cSip, cSip_length, "SETUP rtsp://", "/Streaming", new_ip, new_len);
        }
        return newSip;
    }

    shared_ptr<char> change_rtsp_ip_play(char *cSip, int cSip_length, string new_ip, int &new_len)
    {
        shared_ptr<char> newSip(new char[cSip_length]);
        newSip = change_ip_bewteen_start_end(cSip, cSip_length, "PLAY rtsp://", "/stategrid", new_ip, new_len);
        if (newSip == NULL)
        {
            newSip = change_ip_bewteen_start_end(cSip, cSip_length, "PLAY rtsp://", "/Streaming", new_ip, new_len);
        }
        return newSip;
    }

    shared_ptr<char> change_rtsp_ip_teardown(char *cSip, int cSip_length, string new_ip, int &new_len)
    {
        shared_ptr<char> newSip(new char[cSip_length]);
        newSip = change_ip_bewteen_start_end(cSip, cSip_length, "TEARDOWN rtsp://", "/stategrid", new_ip, new_len);
        if (newSip == NULL)
        {
            newSip = change_ip_bewteen_start_end(cSip, cSip_length, "TEARDOWN rtsp://", "/Streaming", new_ip, new_len);
        }
        return newSip;
    }

    bool is_event_type(char *cSip, int cSip_length, string type)
    {
        char *cStart = (char *)type.c_str();
        int cStart_length = (int)type.length();
        int index_start = 0;
        bool matched = findIndex(cSip, cSip_length, cStart, cStart_length, index_start);
        if (matched)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    shared_ptr<char> change_register_ip(char *cSip, int cSip_length, string new_ip, int &new_len)
    {
        DLOG(INFO) << string(cSip) << endl;
        shared_ptr<char> newSip(new char[cSip_length]);
        newSip = change_ip_bewteen_start_end(cSip, cSip_length, "Via: SIP/2.0/UDP ", ";rport", new_ip, new_len);

        string c1_Start = "Contact: <sip:";
        char *cStart = (char *)c1_Start.c_str();
        int cStart_length = (int)c1_Start.length();
        int index_start = 0;
        bool matched = findIndex(cSip, cSip_length, cStart, cStart_length, index_start);
        if (matched)
        {
            //            index_start = index_start + cStart_length;
        }
        else
        {
            return NULL;
        }

        int c1_length = cStart_length + 19; // "260010000701000000@" = 19
        char c1[c1_length];
        memset(c1, 0, c1_length);
        memcpy(c1, cSip + index_start, c1_length);
        DLOG(INFO) << string(c1) << endl;

        int final_len = 0;
        shared_ptr<char> final_sip = change_from_start(newSip.get(), new_len, c1, c1_length, new_ip + ">", final_len);
        new_len = final_len;

        return final_sip;
    }

    shared_ptr<char> change_notify_ip(char *cSip, int cSip_length, string new_ip, int &new_len)
    {
        DLOG(INFO) << string(cSip) << endl;
        shared_ptr<char> newSip(new char[cSip_length]);
        newSip = change_ip_bewteen_start_end(cSip, cSip_length, "Via: SIP/2.0/UDP ", ";rport", new_ip, new_len);

        string c1_Start = "Contact: <sip:";
        char *cStart = (char *)c1_Start.c_str();
        int cStart_length = (int)c1_Start.length();
        int index_start = 0;
        bool matched = findIndex(cSip, cSip_length, cStart, cStart_length, index_start);
        if (matched)
        {
            //            index_start = index_start + cStart_length;
        }
        else
        {
            return NULL;
        }

        int c1_length = cStart_length + 19; // "260010000701000000@" = 19
        char c1[c1_length];
        memset(c1, 0, c1_length);
        memcpy(c1, cSip + index_start, c1_length);
        DLOG(INFO) << string(c1) << endl;

        int final_len = 0;
        shared_ptr<char> final_sip = change_from_start(newSip.get(), new_len, c1, c1_length, new_ip + ">", final_len);
        new_len = final_len;

        return final_sip;
    }

    shared_ptr<char> change_replay_ip(char *cSip, int cSip_length, string new_ip, int &new_len)
    {
        DLOG(INFO) << string(cSip) << endl;

        int count = get_replay_item_count(cSip, cSip_length);
        DLOG(INFO) << "item count: " << count << endl;

        shared_ptr<char> newSip(new char[cSip_length]);
        memcpy(newSip.get(), cSip, cSip_length);
        int sip_length = cSip_length;

        // change the ip of the every item
        for (int order = 0; order < count; order++)
        {
            string c_start = "rtsp:";
            char *cStart = (char *)c_start.c_str();
            int c_start_length = (int)c_start.length();

            int new_sip_length = 0;
            newSip = change_replay_ip(newSip.get(), sip_length, cStart, c_start_length, new_ip, order, new_sip_length);
            sip_length = new_sip_length;
            if (!newSip.get())
            {
                DLOG(INFO) << "item no matched " << endl;
                return 0;
            }
        }
        DLOG(INFO) << string(newSip.get()) << endl;

        //更改长度
        int new_length = get_SIP_XML_content_length(newSip.get(), sip_length);
        string c3_str = "Content-Length: ";
        char *c3 = (char *)c3_str.c_str();
        int c3_length = (int)c3_str.length();

        int out_length = 0;
        stringstream stream;
        stream << new_length;
        string new_length_str = stream.str();
        shared_ptr<char> final_sip = change_from_start(newSip.get(), sip_length, c3, c3_length, new_length_str, out_length);

        if (!final_sip)
        {
            DLOG(INFO) << "final_sip no matched " << endl;
            return 0;
        }
        //        DLOG(INFO) << string(final_sip.get()) << endl;

        new_len = out_length;
        return final_sip;
    }

    string getVideoPort(char *cSip, int cSip_length)
    {
        //m=video 50004 RTP/AVP 100
        string c = "m=video ";
        int c_length = (int)c.length();
        char cEnd[1] = {0x20};

        int index = 0;
        bool matched = findIndex(cSip, cSip_length, (char *)c.c_str(), c_length, index);
        if (matched)
        {
            index = index + c_length;
            // DLOG(INFO) << "before index: " << index << endl; //after index 是原始port的长度
        }

        int index_end = 0;
        matched = findIndex(cSip + index, cSip_length - index, cEnd, 1, index_end);
        if (matched)
        {
            // DLOG(INFO) << "after index: " << index_end << endl;
        }

        int port_length = index_end;
        // DLOG(INFO) << port_length << endl;
        char port[port_length];
        memcpy(port, cSip + index, port_length);
        //    DLOG(INFO) << string(port) << endl;

        return port;
    }

    string getInviteToIpForDialIp(char *cSip, int cSip_length)
    {
        vector<string> lines;
        SplitString(string(cSip), lines, "\n");

        vector<string> items;
        SplitString(lines[4], items, ":");

        vector<string> items2;
        SplitString(items[2], items2, "@");

        return items2[1];
    }

    shared_ptr<char> change_invite_ip(char *cSip, int cSip_length, string new_ip, int &new_lenth)
    {
        string c1_str = "o=- 0 0 IN IP4 ";
        //6f3d2d2030203020494e2049503420
        //    char c1[15] = {0x6f,0x3d,0x2d,0x20,0x30,0x20,0x30,0x20,0x49,0x4e,0x20,0x49,0x50,0x34,0x20};
        //    int c1_length = 15;
        char *c1 = (char *)c1_str.c_str();
        int c1_length = (int)c1_str.length();

        int fisrt_length = 0;
        shared_ptr<char> first = change_from_start(cSip, cSip_length, c1, c1_length, new_ip, fisrt_length);
        if (!first)
        {
            DLOG(INFO) << "first no matched " << endl;
            return 0;
        }
        //DLOG(INFO) << fisrt_length << endl;
        //        printBuffer(first, fisrt_length, "first");
        //        DLOG(INFO) << string(first.get()) << endl;

        //c=IN IP4
        string c2_str = "c=IN IP4 ";
        //633d494e2049503420
        //    char c2[9] = {0x63,0x3d,0x49,0x4e,0x20,0x49,0x50,0x34,0x20};
        //    int c2_length = 9;
        char *c2 = (char *)c2_str.c_str();
        int c2_length = (int)c2_str.length();

        int second_length = 0;
        shared_ptr<char> second = change_from_start(first.get(), fisrt_length, c2, c2_length, new_ip, second_length);
        if (!second)
        {
            DLOG(INFO) << "second no matched " << endl;
            return 0;
        }
        // DLOG(INFO) << second_length << endl;
        //        printBuffer(second, second_length, "second");
        //        DLOG(INFO) << string(second.get()) << endl;

        //更改长度
        string new_length = invite_new_length(second.get(), second_length);
        //Content-Length:   160
        string c3_str = "Content-Length: ";
        char *c3 = (char *)c3_str.c_str();
        int c3_length = (int)c3_str.length();

        int three_length = 0;
        shared_ptr<char> three = change_from_start(second.get(), second_length, c3, c3_length, new_length, three_length);
        if (!three)
        {
            DLOG(INFO) << "three no matched " << endl;
            return 0;
        }
        //        printBuffer(second, second_length, "second");
        //DLOG(INFO) << string(three.get()) << endl;

        //返回
        new_lenth = three_length;
        return three;
    }

    shared_ptr<char> change_invite_ip_200ok(char *cSip, int cSip_length, string new_ip, string new_ip_port, int &new_len)
    {
        string c1_Start = "Contact: <sip:";
        char *cStart = (char *)c1_Start.c_str();
        int cStart_length = (int)c1_Start.length();
        int index_start = 0;
        bool matched = findIndex(cSip, cSip_length, cStart, cStart_length, index_start);
        if (matched)
        {
            //            index_start = index_start + cStart_length;
        }
        else
        {
            return NULL;
        }

        int c1_length = cStart_length + 19; // "260010000701000000@" = 19
        char c1[c1_length];
        memset(c1, 0, c1_length);
        memcpy(c1, cSip + index_start, c1_length);

        int contact_len = 0;
        shared_ptr<char> contact_sip = change_from_start(cSip, cSip_length, c1, c1_length, new_ip_port + ">", contact_len);
        //        DLOG(INFO) << string(contact_sip.get()) << endl;

        string c1_video_str = "o=- 0 0 IN IP4 ";
        char *c1_video = (char *)c1_video_str.c_str();
        int c1_video_length = (int)c1_video_str.length();

        int fisrt_length = 0;
        shared_ptr<char> first_video = change_from_start(contact_sip.get(), contact_len, c1_video, c1_video_length, new_ip, fisrt_length);
        if (!first_video)
        {
            DLOG(INFO) << "first_video no matched " << endl;
            return NULL;
        }

        //改 Content-Length
        int first_video_new_len = 0;
        shared_ptr<char> first_video_new = change_cotent_length(first_video.get(), fisrt_length, first_video_new_len);
        DLOG(INFO) << string(first_video_new.get()) << endl;

        string c2_video_str = "c=IN IP4 ";
        char *c2_video = (char *)c2_video_str.c_str();
        int c2_video_length = (int)c2_video_str.length();

        int second_length = 0;
        shared_ptr<char> second_video = change_from_start(first_video_new.get(), first_video_new_len, c2_video, c2_video_length, new_ip, second_length);
        if (!second_video)
        {
            DLOG(INFO) << "second_video no matched " << endl;
            return NULL;
        }

        //改 Content-Length
        int second_video_new_len = 0;
        shared_ptr<char> second_video_new = change_cotent_length(second_video.get(), second_length, second_video_new_len);

        new_len = second_video_new_len;
        DLOG(INFO) << string(second_video_new.get()) << endl;

        return second_video_new;
    }

    shared_ptr<char> change_rtsp_sdp_ip(char *cSip, int cSip_length, string new_ip, int &new_lenth)
    {
        string c1_str = "c=IN IP4 ";
        char *c1 = (char *)c1_str.c_str();
        int c1_length = (int)c1_str.length();

        int fisrt_length = 0;
        shared_ptr<char> first = change_from_start(cSip, cSip_length, c1, c1_length, new_ip, fisrt_length);
        if (!first)
        {
            DLOG(INFO) << "first no matched " << endl;
            return 0;
        }

        //改 Content-Length
        int first_video_new_len = 0;
        shared_ptr<char> final_sip = change_cotent_length(first.get(), fisrt_length, first_video_new_len);
        //DLOG(INFO) << string(final_sip.get()) << endl;

        new_lenth = first_video_new_len;
        return final_sip;
    }

    shared_ptr<char> change_rtsp_sdp_ip_2(char *cSip, int cSip_length, string new_ip, int &new_lenth)
    {
        string c1_str = "IN IP4 ";
        char *c1 = (char *)c1_str.c_str();
        int c1_length = (int)c1_str.length();

        int fisrt_length = 0;
        shared_ptr<char> first = change_from_start(cSip, cSip_length, c1, c1_length, new_ip, fisrt_length, 0);
        if (!first)
        {
            DLOG(INFO) << "first no matched " << endl;
            return 0;
        }

        int second_length = 0;
        shared_ptr<char> second = change_from_start(first.get(), fisrt_length, c1, c1_length, new_ip, second_length, 1);
        if (!first)
        {
            DLOG(INFO) << "first no matched " << endl;
            return 0;
        }

        //改 Content-Length
        int video_new_len = 0;
        shared_ptr<char> final_sip = change_cotent_length(second.get(), second_length, video_new_len);
        //DLOG(INFO) << string(final_sip.get()) << endl;

        new_lenth = video_new_len;
        return final_sip;
    }

    shared_ptr<char> change_rtsp_ip_200ok(char *cSip, int cSip_length, string new_ip1, string new_ip2, int &new_lenth)
    {
        shared_ptr<char> newSip(new char[cSip_length]);
        int new_length1;
        newSip = change_ip_bewteen_start_end(cSip, cSip_length, "source=", ";destination=", new_ip1, new_length1);

        shared_ptr<char> newSip2(new char[cSip_length]);
        newSip2 = change_ip_bewteen_start_end(newSip.get(), new_length1, "destination=", ";ssrc=", new_ip2, new_lenth);

        return newSip2;
    }

private:
    shared_ptr<char> change_ip_bewteen_start_end(char *cSip, int cSip_length, string strStart, string strEnd, string new_ip, int &new_lenth)
    {
        string c1_Start = strStart;
        char *cStart = (char *)c1_Start.c_str();
        int cStart_length = (int)c1_Start.length();
        int index_start = 0;
        bool matched = findIndex(cSip, cSip_length, cStart, cStart_length, index_start);
        if (matched)
        {
            index_start = index_start + cStart_length;
            //            DLOG(INFO) << "start: " << index_start << endl;
        }
        else
        {
            return NULL;
        }

        string cEnd_str = strEnd;
        int cEnd_length = (int)cEnd_str.length();
        char *cEnd = (char *)cEnd_str.c_str();

        int index_end = 0;
        matched = findIndex(cSip, cSip_length, cEnd, cEnd_length, index_end);
        if (matched)
        {
        }
        else
        {
            return NULL;
        }

        //原始ip
        int ip_length = index_end - index_start;
        char old_ip[ip_length];
        memcpy(old_ip, cSip + index_start, ip_length);
        //        DLOG(INFO) << "原始ip: " << string(old_ip) << endl;

        //新ip
        char *cNewIp = (char *)new_ip.c_str();
        int new_ip_length = (int)new_ip.length();

        //新sip包的长度，原始sip包的基础上 + ip长度的偏移量
        int new_sip_length = cSip_length + (new_ip_length - ip_length);
        auto new_sip = shared_ptr<char>(new char[new_sip_length]);
        memset(new_sip.get(), 0, new_sip_length);
        new_lenth = new_sip_length; //return

        // step1: 拷贝ip之前的部分
        int offset = 0;
        memcpy(new_sip.get(), cSip, index_start);
        offset += index_start;

        // step2: 拷贝新的ip
        memcpy(new_sip.get() + offset, cNewIp, new_ip_length);
        offset += new_ip_length;

        // step2: 拷贝ip之后的部分
        memcpy(new_sip.get() + offset, cSip + index_end, cSip_length - index_end);
        //    printBuffer(new_sip, new_sip_length, "");
        //        DLOG(INFO) << "新sip: " << string(new_sip.get()) << endl;

        return new_sip;
    }

    int get_replay_item_count(char *cSip, int cSip_length)
    {
        string c1_Start = "SubNum=\"";
        char *cStart = (char *)c1_Start.c_str();
        int cStart_length = (int)c1_Start.length();
        int index_start = 0;
        bool matched = findIndex(cSip, cSip_length, cStart, cStart_length, index_start);
        if (matched)
        {
            //            DLOG(INFO) << "start: " << cSip_length - index_start << endl;
        }
        else
        {
            return 0;
        }

        string cEnd_str = "\" FromIndex=";
        int cEnd_length = (int)cEnd_str.length();
        char *cEnd = (char *)cEnd_str.c_str();

        int index_end = 0;
        matched = findIndex(cSip, cSip_length, cEnd, cEnd_length, index_end);
        if (matched)
        {
            //            DLOG(INFO) << "end: " << index_end << endl;
        }

        int number_length = index_end - index_start - cStart_length;
        char cNumber[number_length];
        memset(cNumber, number_length, 0);
        memcpy(cNumber, cSip + index_start + cStart_length, number_length);
        string srt = string(cNumber);
        //        DLOG(INFO) << string(cNumber) << endl;

        std::string::size_type sz; // alias of size_t
        int count = std::stoi(string(cNumber), &sz);

        return count;
    }

    int get_SIP_XML_content_length(char *cSip, int cSip_length)
    {
        string c1_Start = "<?xml";
        char *cStart = (char *)c1_Start.c_str();
        int cStart_length = (int)c1_Start.length();
        int index_start = 0;
        bool matched = findIndex(cSip, cSip_length, cStart, cStart_length, index_start);
        if (matched)
        {
            //            DLOG(INFO) << "start: " << index_start << endl;
        }
        else
        {
            return 0;
        }

        return cSip_length - index_start;
    }

    shared_ptr<char> change_cotent_length(char *cSip, int cSip_length, int &out_lenth)
    {

        string c1_str = "v=0";
        char *c1 = (char *)c1_str.c_str();
        int c1_length = (int)c1_str.length();

        int index = 0;
        bool matched = findIndex(cSip, cSip_length, c1, c1_length, index);
        if (matched)
        {
            DLOG(INFO) << "new length: " << cSip_length - index << endl;
        }
        else
        {
            return NULL;
        }

        stringstream stream;
        stream << cSip_length - index;
        string new_length = stream.str();

        string c3_str = "Content-Length: ";
        char *c3 = (char *)c3_str.c_str();
        int c3_length = (int)c3_str.length();

        int three_length = 0;
        shared_ptr<char> three = change_from_start(cSip, cSip_length, c3, c3_length, new_length, three_length);
        if (!three)
        {
            DLOG(INFO) << "three no matched " << endl;
            return 0;
        }
        //DLOG(INFO) << string(three.get()) << endl;

        //返回
        out_lenth = three_length;
        return three;
    }

    shared_ptr<char> change_replay_ip(char *cSip, int cSip_length, char *c1, int c1_length, string new_ip, int order, int &new_lenth)
    {
        string c1_Start = "rtsp://";
        char *cStart = (char *)c1_Start.c_str();
        int cStart_length = (int)c1_Start.length();
        int index_start = 0;
        bool matched = findIndex_with_number(cSip, cSip_length, cStart, cStart_length, order, index_start);
        if (matched)
        {
            index_start = index_start + cStart_length;
            //            DLOG(INFO) << "start: " << index_start << endl;
        }
        else
        {
            DLOG(INFO) << "change_replay_ip start not found " << endl;
            return NULL;
        }

        string cEnd_str = "/stategrid";
        int cEnd_length = (int)cEnd_str.length();
        char *cEnd = (char *)cEnd_str.c_str();

        int index_end = 0;
        matched = findIndex_with_number(cSip, cSip_length, cEnd, cEnd_length, order, index_end);
        if (matched)
        {
            //            DLOG(INFO) << "end: " << index_end << endl;
        }
        else
        {
            DLOG(INFO) << "change_replay_ip end not found, try Streaming " << endl;
            cEnd_str = "/Streaming";
            cEnd_length = (int)cEnd_str.length();
            cEnd = (char *)cEnd_str.c_str();

            index_end = 0;
            matched = findIndex_with_number(cSip, cSip_length, cEnd, cEnd_length, order, index_end);
        }

        if (!matched)
        {
            DLOG(INFO) << "change_replay_ip end not found" << endl;
            return NULL;
        }

        //原始ip
        int ip_length = index_end - index_start;
        char old_ip[ip_length];
        memcpy(old_ip, cSip + index_start, ip_length);
        //        DLOG(INFO) << "原始ip: " << string(old_ip) << endl;

        //新ip
        char *cNewIp = (char *)new_ip.c_str();
        int new_ip_length = (int)new_ip.length();

        //新sip包的长度，原始sip包的基础上 + ip长度的偏移量
        int new_sip_length = cSip_length + (new_ip_length - ip_length);
        auto new_sip = shared_ptr<char>(new char[new_sip_length]);
        memset(new_sip.get(), 0, new_sip_length);
        new_lenth = new_sip_length; //return

        // step1: 拷贝ip之前的部分
        int offset = 0;
        memcpy(new_sip.get(), cSip, index_start);
        offset += index_start;

        // step2: 拷贝新的ip
        memcpy(new_sip.get() + offset, cNewIp, new_ip_length);
        offset += new_ip_length;

        // step2: 拷贝ip之后的部分
        memcpy(new_sip.get() + offset, cSip + index_end, cSip_length - index_end);
        //    printBuffer(new_sip, new_sip_length, "");
        //        DLOG(INFO) << "新sip: " << string(new_sip.get()) << endl;

        return new_sip;
    }

    string invite_new_length(char *cSip, int cSip_length)
    {
        //        Content-Length:   160
        //
        //        v=0

        //更改sdp长度
        string c1_str = "v=0";
        char *c1 = (char *)c1_str.c_str();
        int c1_length = (int)c1_str.length();

        int index = 0;
        bool matched = findIndex(cSip, cSip_length, c1, c1_length, index);
        if (matched)
        {
            DLOG(INFO) << "new length: " << cSip_length - index << endl;
        }
        else
        {
            return NULL;
        }

        stringstream stream;
        stream << cSip_length - index;

        return stream.str();
    }

    shared_ptr<char> change_from_start(char *cSip, int cSip_length, char *c1, int c1_length, string new_ip, int &new_lenth, /* bool is_0a = false,*/ int order = -1)
    {

        int index = 0;
        bool matched = true;
        if (order >= 0)
        {

            matched = findIndex_with_number(cSip, cSip_length, c1, c1_length, order, index);
            if (matched)
            {
                index = index + c1_length;
                // DLOG(INFO) << "before index: " << index << endl;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            bool matched = findIndex(cSip, cSip_length, c1, c1_length, index);
            if (matched)
            {
                index = index + c1_length;
                // DLOG(INFO) << "before index: " << index << endl;
            }
            else
            {
                return NULL;
            }
        }

        int index_end = 0;
        char *cEnd = NULL;

        // if (is_0a)
        // {
        //     char cTemp[1] = {0x0a};
        //     cEnd = cTemp;
        // }
        // else
        // {
        //     char cTemp[2] = {0x0d, 0x0a};
        //     cEnd = cTemp;
        // }
        // if (is_0a)
        // {
        //     // printBuffer(cSip + index,cSip_length - index, "is_0a");

        //     matched = findIndex(cSip + index, cSip_length - index, cEnd, 1, index_end);
        // }
        // else
        // {
        //     // printBuffer(cSip + index,cSip_length - index, "is_0d_0a");

        //     matched = findIndex(cSip + index, cSip_length - index, cEnd, 2, index_end);

        // }

        char cTemp[2] = {0x0d, 0x0a};
        cEnd = cTemp;

        matched = findIndex(cSip + index, cSip_length - index, cEnd, 2, index_end);

        if (matched)
        {
            DLOG(INFO) << "after index: " << index_end << endl; //after index 是原始ip的长度
        }
        else
        {
            char cTemp1[1] = {0x0a};
            cEnd = cTemp1;
            matched = findIndex(cSip + index, cSip_length - index, cEnd, 1, index_end);
        }

        //原始ip
        int ip_length = index_end;
        //        DLOG(INFO) << ip_length << endl;
        char old_ip[ip_length];
        memcpy(old_ip, cSip + index, ip_length);
        //        DLOG(INFO) << "原始ip: " << string(old_ip) << endl;

        //新ip
        char *cNewIp = (char *)new_ip.c_str();
        int new_ip_length = (int)new_ip.length();

        //新sip包的长度，原始sip包的基础上 + ip长度的偏移量
        int new_sip_length = cSip_length + (new_ip_length - ip_length);
        auto new_sip = shared_ptr<char>(new char[new_sip_length]);
        memset(new_sip.get(), 0, new_sip_length);
        new_lenth = new_sip_length; //return

        // step1: 拷贝ip之前的部分
        int offset = 0;
        memcpy(new_sip.get(), cSip, index);
        offset += index;

        // step2: 拷贝新的ip
        memcpy(new_sip.get() + offset, cNewIp, new_ip_length);
        offset += new_ip_length;

        // step2: 拷贝ip之后的部分
        memcpy(new_sip.get() + offset, cSip + index + index_end, cSip_length - index - index_end);
        //    printBuffer(new_sip, new_sip_length, "");

        return new_sip;
    }

    int findIndex(char *source, int source_size, char *target, int target_size, int &index)
    {
        bool matched = false;
        for (int i = 0; i < source_size; i++)
        {
            for (int j = 0; j < target_size; j++)
            {
                //            DLOG(INFO) << target[j] << endl;
                //            DLOG(INFO) << source[i + j] << endl;
                if (target[j] == source[i + j])
                {
                    matched = true;
                    index = i;
                }
                else
                {
                    matched = false;
                    break;
                }
            }
            if (matched)
            {
                break;
            }
        }
        return matched;
    }

    int findIndex_with_number(char *source, int source_size, char *target, int target_size, int order, int &index)
    {
        bool matched = false;
        int temp_order = 0;
        for (int i = 0; i < source_size; i++)
        {
            for (int j = 0; j < target_size; j++)
            {
                //            DLOG(INFO) << target[j] << endl;
                //            DLOG(INFO) << source[i + j] << endl;
                if (target[j] == source[i + j])
                {
                    matched = true;
                    index = i;
                }
                else
                {
                    matched = false;
                    break;
                }
            }
            if (matched && order == temp_order)
            {
                break;
            }
            else if (matched && order != temp_order)
            {
                temp_order++;
            }
        }
        return matched;
    }

    void SplitString(const string &s, vector<string> &v, const string &c)
    {
        string::size_type pos1, pos2;
        pos2 = s.find(c);
        pos1 = 0;
        while (string::npos != pos2)
        {
            v.push_back(s.substr(pos1, pos2 - pos1));

            pos1 = pos2 + c.size();
            pos2 = s.find(c, pos1);
        }
        if (pos1 != s.length())
            v.push_back(s.substr(pos1));
    }

    string insertRN(string target)
    {
        char r = 0x0a;
        char n = 0x0d;
        return target + n + r;
    }

    string change_ip(string source_line, string ip_before, string ip_after, string ip, int offset_before = 0)
    {
        string line_result = "";
        int index = source_line.find(ip_before);
        string s1 = source_line.substr(0, index + 1 + offset_before);
        index = source_line.find(ip_after);
        if (index > 0)
        {
            string s2 = source_line.substr(index, source_line.length() - 1);
            line_result = s1 + ip + s2;
            return line_result;
        }
        else
        {
            DLOG(INFO) << "change_ip failed: " << source_line << endl;
            return source_line;
        }
    }
};

#endif /* siputil_hpp */
