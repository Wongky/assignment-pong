#include "bluetooth.h"

void Job(Pit*){
	/*code which will execute periodically*/

}

//initialize member bluetooth and member pit in this constructor
Bluetooth::Bluetooth()
{
	//Config con;
	std::function<bool(const Byte *data, const size_t size)> f1 = Listener;
	JyMcuBt106::Config uartConfig = Config::GetBluetoothConfig(f1);

	//m_bt.Initializer(uartConfig);
	m_bt = JyMcuBt106(uartConfig);

	std::function<void(Pit*)> f2 = Job;
	Pit::Config pitConfig = Config::GetBluetoothPitConfig(f2);
	m_pit = Pit(pitConfig);


}

//implementation of sending out bytes
void Bluetooth::SendBuffer(const Byte *buff, const size_t &size)
{
	m_bt.SendBuffer(buff,size);
}

/**
 * you need to resend package per 10ms until receive ACK
 * this function set enable this proccess or not
 */
void Bluetooth::EnableTimer(bool flag)
{
	is_timer_enabled=flag;
}
