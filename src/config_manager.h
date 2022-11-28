//
// Created by qianlu on 2022/11/28.
//

#ifndef OT_DEMO_CONFIG_MANAGER_H
#define OT_DEMO_CONFIG_MANAGER_H

#include "nacos/Nacos.h"
#include "nacos/Properties.h"
#include "nacos/config/ConfigService.h"
#include "nacos/listen/Listener.h"
#include "iostream"
#include "unordered_map"
#include "variant"

#include "gflags/gflags_declare.h"

DECLARE_string(config_server_addr);

using namespace nacos;
using namespace std;

class ConfigManager {

public:
    ConfigManager() {
    }

    virtual void init() {}

    virtual void listen() {}

    virtual void publish() {}

    virtual std::string get(std::string_view& key) {}
private:

};

class NacosConfigManager : public ConfigManager {
public:
    NacosConfigManager(){
        props[PropertyKeyConst::SERVER_ADDR] = FLAGS_config_server_addr;
    }

    void init() override {
        INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(props);
        configService = factory->CreateConfigService();
        guard = new ResourceGuard<ConfigService>(configService);
        // get all config content
        NacosString configService->getConfig();


    }

    void listen() override {
        listener = new MyListener(1);//You don't need to free it, since it will be deleted by the function removeListener
        configService->addListener("dqid", NULLSTR, listener);//All changes on the key dqid will be received by MyListener
    }

    void publish() override {

    }

    std::string get(std::string_view& key) override {

    }

    ~NacosConfigManager() {
        cout << "remove listener" << endl;
        configService->removeListener("dqid", NULLSTR, listener);//Cancel listening
        delete guard;
    }
private:

    class MyListener : public Listener {
    private:
        int num;
    public:
        MyListener(int num) {
            this->num = num;
        }

        void receiveConfigInfo(const NacosString &configInfo) {
            cout << "===== config chagned! =====" << endl;
            cout << configInfo << endl;
            cout << "===========================" << endl;
        }
    };

    Properties props;
    ConfigService* configService;
    ResourceGuard<ConfigService>* guard;
    MyListener* listener;



};


#endif //OT_DEMO_CONFIG_MANAGER_H
