#ifndef SOCKETCONSTANT_H
#define SOCKETCONSTANT_H

#define INIT_NUMBER             -1  //初始化数据

#define NET_PACKET_START        "~"
#define NET_PACKET_END          "~"
#define ESCAPE_CHARACTER_SRC1   "}"
#define ESCAPE_CHARACTER_DST1   "}\1"
#define ESCAPE_CHARACTER_SRC2   "~"
#define ESCAPE_CHARACTER_DST2   "}\2"

#define NET_PACHET_START_SIZE   1
#define NET_PACHET_HEADERS_SIZE 12
#define NET_PACHET_EFFECT_SIZE  1
#define NET_PACHET_END_SIZE     1
#define ESCAPE_CHARACTER_SRC1_SIZE  1
#define ESCAPE_CHARACTER_DST1_SIZE  2
#define ESCAPE_CHARACTER_SRC2_SIZE  1
#define ESCAPE_CHARACTER_DST2_SIZE  2

//Socket Message ID
#define CLIENT_GENERAL_REPLY    0x0001  //终端通用应答
#define SERVE_GENERAL_REPLY     0x8001  //服务器通用应答
#define CLIENT_HEARTBEAT        0x0F01  //终端心跳
#define CLIENT_SIGN_IN          0x0F00  //终端注册
#define SERVE_SIGN_IN_REPLY     0x8F00  //服务器注册应答
#define CLIENT_AUTHENTICATION   0x0F02  //终端鉴权
#define CLIENT_LOGOUT           0x0F03  //终端注销
#define SERVE_SET_CLIENT_PARA   0x8F04  //服务器设置终端参数
#define SERVE_QUERY_CLIENT_PARA 0x8F05  //服务器查询终端参数
#define CLIENT_QUERY_PARA_REPLY 0x0F05  //终端查询参数应答
#define SERVE_SPECIFY_QUERY_CLIENT_PARA 0x8F06  //服务器查询指定参数
#define SERVE_QUERY_CLIENT_INFO 0x8F07  //服务器查询终端信息
#define CLIENT_QUERY_INFO_REPLY 0x0F07  //终端查询信息应答
#define SERVE_FILE_UPDATE       0x8F08  //文件更新
#define CLIENT_FILE_UPDATE_RESULT   0x0F08  //文件更新结果
#define CLIENT_ERROR_REPORT     0x0F09  //终端错误上报
#define SERVE_PERIPHERAL_CONTROL    0x8F0A  //服务器外设控制
#define CLIENT_PERIPHERAL_CONTROL_REPLY 0x0F0A  //终端外设控制应答
#define CLIENT_COORD_REPORT     0x0F0B  //终端定位信息上报
#define CLIENT_CAN_REPORT       0x0F0C  //终端can数据上传
#define SERVE_DATA_TRANSPARENT  0x8F0D  //数据下行透传（服务器-终端）
#define CLIENT_DATA_TRANSPARENT 0x0F0D  //数据上行透传（终端-服务器）

//Client Result
#define RESULT_OK   0
#define RESULT_FIAL 1

//Para ID
#define PARA_NUMER                  16
#define PARA_HEARTBEAT              0x0001
#define PARA_IP                     0x0002
#define PARA_PORT                   0x0003
#define PARA_REPLY_TIMEOUT          0x0004
#define PARA_RETRANS                0x0005
#define PARA_SPEED                  0x0006
#define PARA_OVERSPEED_TIME         0x0007
#define PARA_CAN_ACQUISITION        0x0008
#define PARA_CAN_UPLOAD             0x0009
#define PARA_GPS_UPLOAD             0x0010
#define PARA_PROVINCE_ID            0x0080
#define PARA_CITY_ID                0x0081
#define PARA_LICENSE_PLATE          0x0082
#define PARA_LICENSE_PLATE_COLOR    0x0083
#define PARA_DELAY_TIME             0x0084
#define PARA_LOGIN_PASS             0x0085

//Transparent type
#define LCD_TRANSPARENT 0x01

#endif // SOCKETCONSTANT_H
