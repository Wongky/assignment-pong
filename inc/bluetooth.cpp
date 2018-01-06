#include "bluetooth.h"

Bluetooth::~Bluetooth()
{

}
/*
//initialize member bluetooth and member pit in this constructor
Bluetooth::Bluetooth()
{
	//Config con;
	//std::function<bool(const Byte *data, const size_t size)> f1 = Listener;
	//JyMcuBt106::Config uartConfig = Config::GetBluetoothConfig(f1);
	//JyMcuBt106::Config uartConfig = Config::GetBluetoothConfig(Listener);
	//m_bt.Initializer(uartConfig);
	//m_bt = JyMcuBt106(uartConfig);

	//std::function<void(Pit*)> f2 = Job;
	//Pit::Config pitConfig = Config::GetBluetoothPitConfig(f2);
	//Pit::Config pitConfig = Config::GetBluetoothPitConfig(Job);
	//m_pit = Pit(pitConfig);


}
*/
//implementation of sending out bytes
void Bluetooth::SendBuffer(const Byte *buff, const size_t &size)
{
	m_bt.SendBuffer(buff,size);
}

/**
 * deliver the first package in queue
 */
virtual void Bluetooth::SendFirst()
{
	if(!queue.empty())
	{
		Package temp_package = queue[1];
		//resend is not needed
		if(!IsTimerEnable())
		{
			//pop the first package
			queue.erase(queue.begin());
		}


		//http://zh.cppreference.com/w/cpp/types/size_t
		//calculate the size of buffer array to be send
		size_t size = 3 + temp_package.data.size(); //sum of frame id , PkgType, labeling end of package, data size

		//construct send buffer
		Byte buff[size];

		//buff[0] = temp_package.frame_id;
		//buff[1] = temp_package.type;
		memcpy(buff,&temp_package.frame_id,1);
		memcpy(buff+1,&temp_package.type,1);

		int i = 2;
		Byte buffer_data;
		for (std::vector<Byte>::iterator it = temp_package.data.begin(); it != temp_package.data.end(); ++it, ++i)
		{
			//buff[i] = *it;
			buffer_data = *it;
			memcpy(buff+i,&buffer_data,1);
		}
		//buff[size-1] = labelDetermine(int(temp_package.type));
		PkgType temp_type = labelDetermine(temp_package.type);
		memcpy(buff+(size-1),&temp_type,1);

		//set send_time should be on main?
		//send_time=System::Time();
		//send buffer
		SendBuffer(buff, size);
	}
}

/**
 * you need to resend package per 10ms until receive ACK
 * this function set enable this proccess or not
 */
void Bluetooth::EnableTimer(bool flag)
{
	is_timer_enabled=flag;
	m_pit.SetEnable(flag); //enable resend package per 10ms until receive ACK proccess
}

/*
bool Bluetooth::blueListener(const Byte* data, const size_t size)
{
	bool flag = Listener(data,size);

	//disable resend if ack received;
	if(IsTimerEnable())
	{
		if(!IsWaitingACK())
		{
			EnableTimer(false);
		}
	}
	return flag;
}
*/
