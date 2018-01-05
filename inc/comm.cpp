#include "comm.h"

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
virtual Comm::~Comm()
{

}

/*
 * determine the ending label of package based on pkgType
 * return 0 is error
 */
//Byte Comm::labelDetermine(const Comm::PkgType& t)
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
	Package temp_package = pkg;
	
	do{
		if(System::Time()-send_time()>10){
			queue.push_back(pkg); //push the package to pending send package
			SendFirst(); //send the first package in queue
		}
		//every 10ms
	}while(need_ack && is_waiting_ack); //need ack and waiting for ack


}

/**
 * Listener (will be irs for bluetooth) when bunch of Bytes arrive
 * You may want to parse the Byte (uint8_t) array into packages
 * return true if the data is consumed
 */
bool Comm::Listener(const Byte* data, const size_t size)
{
	//put data to buffer, cover from beginning
	//https://stackoverflow.com/questions/1951519/when-should-i-use-stdsize-t
	size_t i = 0;
	for(std::vector<Byte>::iterator it = buffer.begin();i<size;i++, it++)
	{
		buffer.insert(it,data[i]);
	}

	//parse the byte if end of data is end label
	if(data[size-1]== BitConsts::kSTART||data[size-1]== BitConsts::kEND||data[size-1]== BitConsts::kACK)
	{
		BuildBufferPackage(size);
	}
	return true;
}


/**
 * deliver the first package in queue
 */
virtual void Comm::SendFirst()
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
		send_time=System::Time();
		//send buffer
		SendBuffer(buff, size);
	}
}

/**
 * when the buffer contains a complete package bytes,
 * build the package, call handler/ handle the packages directly
 * and clear buffer
 */
void Comm::BuildBufferPackage(const size_t& size)
{
	if(buffer.empty())return;
	if(buffer.size()<size)return;

	//check if package format
	if(buffer[size-1]!= BitConsts::kSTART||buffer[size-1]!= BitConsts::kEND||buffer[size-1]== BitConsts::kACK)
	{
		switch(buffer[size-1])
		{
		case BitConsts::kSTART:
		{
			if(size!=3)return; //check package size
			if(buffer[1]!=0)return; //check msg type (start msg is 0)
			if(buffer[0]!=0)return; //check frame ID (start frame ID is 0)

			//build package
			Package temp_package{0,0,{}};
			//clear buffer
			buffer.clear();
			//call package handler
			PackageHandler(temp_package);
			break;
		}
		case BitConsts::kACK:
		{
			if(size!=3)return; //check package size
			if(labelDetermine(buffer[1])!=BitConsts::kACK)return; //check msg type is for ack

			//build package
			Package temp_package{buffer[0],buffer[1],{}};
			//clear buffer
			buffer.clear();
////////////may not correct
			//receive ack
			is_waiting_ack = false;
			//call package handler
			PackageHandler(temp_package);
			break;
		}
		case BitConsts::kEND:
		{
			if(size<3)return; //check package size
			if(labelDetermine(buffer[1])!=BitConsts::kEND)return; //check msg type is for ack
			//check enough data
			if(buffer[1]==2||buffer[1]==4||buffer[1]==10)
			{
				if(size!=4)return;
			}
			else if(buffer[1]==6||buffer[1]==8)
			{
				if(size!=5)return;
			}

			//build package
			Package temp_package{buffer[0],buffer[1],{}};
			for(size_t i = 2; i<size;i++)
			{
				temp_package.data.push_back(buffer[i]);
			}
			//clear buffer
			buffer.clear();
			//call package handler
			PackageHandler(temp_package);
			break;
		}
		}
	}
/*
	//check if package format
	if(buffer[buffer.size()-1]!= BitConsts::kSTART||buffer[buffer.size()-1]!= BitConsts::kEND||buffer[buffer.size()-1]== BitConsts::kACK)
	{
		switch(buffer[buffer.size()-1])
		{
		case BitConsts::kSTART:
		{
			if(buffer.size()<3)return; //check package size
			if(buffer[buffer.size()-2]!=0)return; //check msg type (start msg is 0)
			if(buffer[buffer.size()-3]!=0)return; //check frame ID (start frame ID is 0)

			//build package
			Package temp_package{0,0,{}};
			//clear buffer
			buffer.clear;
			//call package handler
			PackageHandler(temp_package);
			break;
		}
		case BitConsts::kACK:
		{
			if(buffer.size()<3)return; //check package size
			if(labelDetermine(buffer[buffer.size()-2])!=BitConsts::kACK)return; //check msg type is for ack

			//build package
			Package temp_package{buffer[buffer.size()-3],buffer[buffer.size()-2],{}};
			//clear buffer
			buffer.clear;
			//call package handler
			PackageHandler(temp_package);
			break;
		}
		case BitConsts::kEND:
		{
			if(buffer.size()<3)return; //check package size
			if(labelDetermine(buffer[buffer.size()-2])!=BitConsts::kEND)return; //check msg type is for ack

			//build package
			//one data
			if(buffer[buffer.size()-2]==2||buffer[buffer.size()-2]==4||buffer[buffer.size()-2]==10)
			{
				Package temp_package{buffer[buffer.size()-3],buffer[buffer.size()-2],{}};
				PackageHandler(temp_package);
			}
			Package temp_package{buffer[buffer.size()-3],buffer[buffer.size()-2],{}};
			PackageHandler(temp_package);
			break;
		}
		}
	}
*/

}
