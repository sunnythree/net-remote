/************************************************************
** FileName: my_dfb.c
**Author:      Liu Jinwei
**Version :    1.0
**Company:  KonKa
**Date:           2015.12.16
**Description:   direct operate functions,product remote control event to MUF
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
***********************************************************/

#include <my_dfb.h>
#include <stdio.h>
#include <stdlib.h>

IDirectFB *dfb=NULL;
IDirectFBInputDevice *dfbDevice = NULL;

/*************************************************
**Function: key2symbol
**Description: ekey to keysymbol
**Input: ekey --->value present a key
**Output:  NULL
**Return: DFBInputDeviceKeySymbol --->a keysymbol native mechine can recognise
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
*************************************************/

DFBInputDeviceKeySymbol key2symbol(int ekey)
{
	DFBInputDeviceKeySymbol ret;
//check target /config/mufirkeymap file
	switch(ekey){
		case 0xf0:
			ret=0xf068;//input
			break;
		case 0xf1:
			ret=0xf0AA;//youtube
			break;
		case 0xf2:
			ret=0xf082;//Qmenu
			break;
		case 0xf3:
			ret=0x000d;//selected
			break;
		case 0xf4:
			ret=0xf048;//return or back
			break;
		case 0x2a:
			ret=DIKS_CHANNEL;
			break;
		case 0x25:
			ret=DIKS_CHANNEL_FAV_LIST;
			break;
		case 0xe:
			ret=DIKS_CHANNEL_RETURN;
			break;
		case 0x11:
			ret=DIKS_CHANNEL_UP;
			break;
		case 0x10:
			ret=DIKS_CHANNEL_DOWN;
			break;
		case 0xd:
			ret=DIKS_SOUND_MODE;
			//ret=DIKS_AUDIO;
			break;
		case 0x13:
			ret=DIKS_VOLUME_UP;
			break;
		case 0x12:
			ret=DIKS_VOLUME_DOWN;
			break;
//		case 0xff:
//			break;
		case 0x2b:
			ret=DIKS_CURSOR_UP;
			break;
		case 0x2c:
			ret=DIKS_CURSOR_DOWN;
			break;
		case 0x2d:
			ret=DIKS_CURSOR_LEFT;
			break;
		case 0x2e:
			ret=DIKS_CURSOR_RIGHT;
			break;
		case 0xb:
			ret=DIKS_POWER;
			break;
		case 0x30:
			ret=DIKS_EXIT;
			break;
		case 0x15:
			ret=DIKS_MENU;
			break;
		case 0x2f:
			ret=DIKS_RETURN;
			break;
		case 0x0:
			ret=DIKS_0;
			break;
		case 0x1:
			ret=DIKS_1;
			break;
		case 0x2:
			ret=DIKS_2;
			break;
		case 0x3:
			ret=DIKS_3;
			break;
		case 0x4:
			ret=DIKS_4;
			break;
		case 0x5:
			ret=DIKS_5;
			break;
		case 0x6:
			ret=DIKS_6;
			break;
		case 0x7:
			ret=DIKS_7;
			break;
		case 0x8:
			ret=DIKS_8;
			break;
		case 0x9:
			ret=DIKS_9;
			break;
		case 0x14:
			ret=DIKS_MUTE;
			break;
		case 0x27:
			ret=DIKS_PAGE_UP;
			break;
		case 0x24:
			ret=DIKS_PAGE_DOWN;
			break;
		case 0xa:
			ret=DIKS_INFO;
			break;
		case 0x1a:
			ret=DIKS_RED;
			break;
		case 0x1f:
			ret=DIKS_BLUE;
			break;
		case 0xc:
			ret=DIKS_YELLOW;
			break;
		case 0x16:
			ret=DIKS_GREEN;
			break;
		case 0x1c:
			ret=DIKS_SOURCE;
			break;
		case 0x22:
			ret=DIKS_EPG;
			break;
		case 0x17:
			ret=DIKS_AUDIO;
			//ret=DIKS_MTS;
			break;
//		case 0xff: //3
//			break;
		case 0x1e:
			ret=DIKS_ZOOM;
			break;
//		case 0xff: //12
//			break;
		case 0x38:
			ret=DIKS_SUBTITLE;
			break;
//		case 0xff: //2
//			break;
		case 0x19:
			ret=DIKS_FREEZE;
			break;
//		case 0xff: //5
//			break;
		case 0x1d:
			ret=DIKS_PICTURE_MODE;
			//ret=DIKS_REVEAL;
			break;
//		case 0xff: //2
//			break;
		case 0x36:
			ret=0xf100;		//DFB_FUNCTION_KEY(0) not defined in dfb
			break;
		case 0x32:
			ret=DIKS_F1;
			break;
		case 0x33:
			ret=DIKS_F2;
			break;
		case 0x34:
			ret=DIKS_F3;
			break;
		case 0x31:
			ret=DIKS_F4;
			break;
		case 0x41:
			ret=DIKS_F5;
			break;
		case 0x39:
			ret=DIKS_F6;
			break;
		case 0x42:
			ret=DIKS_F7;
			break;
		case 0x40:
			ret=DIKS_F8;
			break;
		case 0x18:
			ret=DIKS_F9;
			break;
		case 0x37:
			ret=DIKS_F10;
			break;
		//add by liying 2012-4-10
		case 0x60:
			ret=DIKS_MTS;
			break;
		case 0x61:
			ret=DIKS_SLEEP;
			break;
		case 0x62:
			ret=DIKS_CC;
			break;
		case 0x63:
			ret=DIKS_INTERNET;
			break;
		case 0x64:
			ret=DIKS_RECORD;
			break;
		case 0x65:
			ret=DIKS_STOP;
			break;
		case 0x66:
			ret=DIKS_PLAYPAUSE;
			break;
		case 0x67:
			ret=DIKS_FORWARD;
			break;
		case 0x68:
			ret=DIKS_BACKWARD;
			break;
		case 0x69:
			ret=DIKS_PREVIOUS;
			break;
		case 0x6A:
			ret=DIKS_NEXT;
			break;
		case 0x6B:
			ret=DIKS_EPG;
			break;
		case 0x6C:
			ret=DIKS_DASH;
			break;
		case 0x6D:
			ret=DIKS_BACK;
			break;
		default:
			ret=DIKS_NULL;
			break;
	}
	return ret;
}

/*************************************************
**Function: key2id
**Description: ekey to keyid
**Input: ekey --->value present a key
**Output:  NULL
**Return: DFBInputDeviceKeySymbol --->a keyid native mechine can recognise
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
*************************************************/

DFBInputDeviceKeyIdentifier key2id(int ekey)
{
	DFBInputDeviceKeyIdentifier ret;
//check target /config/mufirkeymap file
	switch(ekey){
		case 0x6D:
			ret=DIKI_R;
			break;
		case 0x30:
			ret=DIKI_X;
			break;
		case 0x2b:
			ret=DIKI_UP;
			break;
		case 0x2c:
			ret=DIKI_DOWN;
			break;
		case 0x2d:
			ret=DIKI_LEFT;
			break;
		case 0x2e:
			ret=DIKI_RIGHT;
			break;
		case 0x2f:
			ret=DIKI_ENTER;
			break;
		case 0x0:
			ret=DIKI_0;
			break;
		case 0x1:
			ret=DIKI_1;
			break;
		case 0x2:
			ret=DIKI_2;
			break;
		case 0x3:
			ret=DIKI_3;
			break;
		case 0x4:
			ret=DIKI_4;
			break;
		case 0x5:
			ret=DIKI_5;
			break;
		case 0x6:
			ret=DIKI_6;
			break;
		case 0x7:
			ret=DIKI_7;
			break;
		case 0x8:
			ret=DIKI_8;
			break;
		case 0x9:
			ret=DIKI_9;
			break;
		default:
			ret=DIKI_UNKNOWN;
			break;
	}
	return ret;
}

/*************************************************
**Function: dfb_sendir
**Description: send remote control events
**Input: ekey --->value present a key
**Output:  NULL
**Return: NULL
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
*************************************************/

void dfb_sendir(int ekey)
{
	DFBInputEvent  evt;
	InputDeviceIoctlData sendkeyparam;
	printf("enter send ir ekey=%d\n",ekey);
	sendkeyparam.request = DFB_DEV_IOC_SEND_LOOPBACK_EVENT; 
	evt.flags = (DFBInputEventFlags) (DIEF_KEYCODE | DIEF_KEYID | DIEF_KEYSYMBOL);
	evt.type = DIET_KEYPRESS;
	evt.key_id = key2id(ekey);//(DFBInputDeviceKeyIdentifier)0; 
	evt.key_symbol = key2symbol(ekey);
	evt.key_code = 0xdd;
	printf("ryan:(%s)%s:%x:%lx:%x\n",__FUNCTION__,"send ir event to dfb",evt.key_id,(unsigned long int)evt.key_symbol,evt.key_code);
	memcpy(sendkeyparam.param, &evt, sizeof(DFBInputEvent));
	dfbDevice->IOCtl(dfbDevice, &sendkeyparam);
	evt.type = DIET_KEYRELEASE;
	memcpy(sendkeyparam.param, &evt, sizeof(DFBInputEvent));
	dfbDevice->IOCtl(dfbDevice, &sendkeyparam);
}

/*************************************************
**Function: input_device_callback
**Description: it will be called when a input device find every times;
**Input: DFBInputDeviceID id  -->present a device
**DFBInputDeviceDescription desc  -->information of device
**void* data                                         -->
**Output:  NULL
**Return: DFBEnumerationResult  --> return  status;
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
*************************************************/

DFBEnumerationResult input_device_callback(DFBInputDeviceID id, DFBInputDeviceDescription desc, void* data)
{

       printf("desc.name: %s\n",desc.name);

       if(strcmp(desc.name, "mstarloopback") == 0) 
       {
           if( DFB_OK==dfb->GetInputDevice(dfb,id,&dfbDevice))
            {
                 printf(" find  loopback  device\n");
            }

       }
       return DFENUM_OK;
}

/*************************************************
**Function: directfb_init
**Description: directfb init function
**Input: NULL
**Output:  NULL
**Return: NULL
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
*************************************************/

void directfb_init()
{
	DFBResult dfb_ret;
	DirectFBInit(NULL,NULL);
	DirectFBCreate(&dfb);
	dfb_ret = dfb->EnumInputDevices( dfb, input_device_callback, NULL );
}
