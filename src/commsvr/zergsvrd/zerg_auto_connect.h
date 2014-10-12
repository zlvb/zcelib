
#ifndef ZERG_SERVER_AUTO_CONNECT_H_
#define ZERG_SERVER_AUTO_CONNECT_H_



class Zerg_Server_Config;



/****************************************************************************************************
class Zerg_Auto_Connector
****************************************************************************************************/
class Zerg_Auto_Connector
{

public:

    //构造函数
    Zerg_Auto_Connector();
    ~Zerg_Auto_Connector();

    ///读取配置
    int get_config(const Zerg_Server_Config *config);

    // 重新加载主动连接配置
    int reload_cfg(const Zerg_Server_Config *config);


    /*!
    * @brief      链接所有的服务器,如果已经有链接，就跳过,
    * @return     void
    * @param      szvalid  仍然有效的的链接
    * @param      sz_succ  成功开始连接的服务器个数
    * @param      szfail   链接失败的服务器数量，但是由于是异步链接，这个地方并不一定真是进行了链接
    * @note
    */
    void reconnect_allserver(size_t &szvalid, size_t &sz_succ, size_t &szfail);


    //链接的数量
    size_t numsvr_connect();


    /*!
    * @brief      根据SVC ID,检查是否是主动连接的服务.,
    * @return     int
    * @param      reconnect_svcid 要进行重连的主路由信息
    * @note       为什么不把一个TCP_Svc_Handler作为参数返回,因为在发起Connect过程中,也可能handle_close.
    */
    int reconnect_server(const SERVICES_ID &reconnect_svcid);


    /*!
    * @brief      根据services_type查询对应的配置主备服务器列表数组 MS（主备）
    * @return     int == 0 表示成功
    * @param[in]  services_type
    * @param[out] ms_svcid_ary   配置的主备服务器列表数组
    */
    int find_conf_ms_svcid_ary(uint16_t services_type,
                               std::vector<uint32_t> *& ms_svcid_ary);


    inline bool is_auto_connect_svcid(const SERVICES_ID &svc_id)
    {
        //如果在SET里面找不到
        if (set_auto_cnt_svcid_.find(svc_id) == set_auto_cnt_svcid_.end())
        {
            return false;
        }
        return true;
    }
protected:

    //根据SVRINFO+IP,检查是否是主动连接的服务.并进行连接
    int connect_server_bysvcid(const SERVICES_ID &svc_id,
                               const ZCE_Sockaddr_In &inet_addr);

protected:

    //
    typedef std::vector<SERVICES_INFO > ARRAY_OF_ZERG_SVCINFO;
    //
    typedef std::unordered_set<SERVICES_ID, HASH_OF_SVCID> SET_OF_SVCID;

    ///类型对应的SERVICES ID 数组的MAP的类型,
    typedef std::unordered_map<uint16_t, std::vector<uint32_t> > MAP_OF_TYPE_TO_IDARY;

protected:

    //连接器
    ZCE_Socket_Connector zerg_connector_;

    //配置实例指针
    const Zerg_Server_Config *zerg_svr_cfg_ = NULL;

    ///
    size_t size_of_wantconnect_ = 0;
    ///
    ARRAY_OF_ZERG_SVCINFO    ary_want_connect_;
    ///
    SET_OF_SVCID set_auto_cnt_svcid_;;

    //类型对应的SERVICES ID 数组 的MAP，数组里面的id的顺序其实是主备顺序
    MAP_OF_TYPE_TO_IDARY     type_to_idary_map_;

};


#endif //_ZERG_SERVER_AUTO_CONNECT_H_


