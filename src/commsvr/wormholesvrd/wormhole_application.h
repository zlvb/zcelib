#ifndef WORMHOLE_APPLICATION_H_
#define WORMHOLE_APPLICATION_H_

#include "wormhole_proxyprocess.h"



/*!
* @brief      
*             
* @note       
*/
class Wormhole_Proxy_App : public Soar_SvrdApp_ZergBus
{

public:
    // 自己的单子偷偷藏着，
    Wormhole_Proxy_App();
    ~Wormhole_Proxy_App();

public:

    //on_run 函数，在基类已经实现。

    /// 处理收到的APPFRAME,不使用const的原因是因为为了加快速度，很多地方是直接将recv_frame修改
    virtual int process_recv_frame(Zerg_App_Frame *recv_frame);

    ///初始化,根据启动参数启动
    virtual int on_start(int argc, const char *argv[]);

    ///处理退出的清理工作
    virtual int on_exit();

protected:
    ///重新加载配置
    virtual int reload_config();

protected:

    /// 处理的PROXY的接口
    Interface_WH_Proxy *interface_proxy_ = NULL;



};

#endif //WORMHOLE_APPLICATION_H_

