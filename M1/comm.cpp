#include "comm.h"
#include "bluetooth.h"

/**
 * Constructor and destructor
 */
Comm::Comm()
{
	//tx variables
	//vector<Package> queue;	//pending send package
	send_time = 0;			//last send time

	is_waiting_ack = false;

	//PackageHandler Handler;  //package handler function (function<void(Package)>)
	//rx veriable
	//vector<Byte> buffer;	//temporary storage for incoming Bytes
}
Comm::~Comm()
{

}

/*
 * determine the ending label of package based on pkgType
 * return 0 is error
 */
//Byte Comm::labelDetermine(const Comm::PkgType& t)
/*
Byte Comm::labelDetermine(const int t)
{
	switch(t)
	{
	case 0:
		return Comm::BitConsts::kSTART;
	case 2: case 4: case 6: case 8: case 10:
		return Comm::BitConsts::kEND;
	case 1: case 3: case 5: case 7: case 9: case 11:
		return Comm::BitConsts::kACK;
	default:
		return 0;
	}
}
*/
//TX helpers
/**
 * SendPackage will send package
 * if need_ack, keep sending package until receive ack
 * else send out the package directly
 * tips: use a queue
 */
void Comm::SendPackage(const Package& pkg, bool need_ack = true)
{
	if(need_ack)is_waiting_ack = true;
	//Package temp_package = pkg;
	
	queue.push_back(pkg); //push the package to pending send package
	/*
	//push total 10 same package to pending send package for resend
	for(int i = 1; i<10 && need_ack;i++)
	{
		queue.push_back(pkg); //push the package to pending send package
	}
	*/

	if(!need_ack){SendFirst();}//send the first package in queue

}

/**
 * Listener (will be irs for bluetooth) when bunch of Bytes arrive
 * You may want to parse the Byte (uint8_t) array into packages
 * return true if the data is consumed
 */


/**
 * deliver the first package in queue
 */
void Comm::SendFirst()
{
	if(!queue.empty())
	{
		//pop the first package
		Package temp_package = queue[1];
		queue.erase(queue.begin());

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
 * when the buffer contains a complete package bytes,
 * build the package, call handler/ handle the packages directly
 * and clear buffer
 */


