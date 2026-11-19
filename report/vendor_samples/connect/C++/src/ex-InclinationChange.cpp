#include "ATKConnectorlib.h"

int main()
{

    // 与 ATK 进行连接
    int conID = atkOpen("127.0.0.1", 6655);

    // 新建一个名为 InclinationChange 的场景
    atkConnect(conID, "New", "/ Scenario InclinationChange");

    // 新建一个名为 SatInclinationChange 的卫星
    atkConnect(conID, "New", "/ Satellite SatInclinationChange");

    //保存场景
    atkConnect(conID, "Save", "/ *");

    //与ATK断开连接
    atkClose(conID);
    
    return 0;
}