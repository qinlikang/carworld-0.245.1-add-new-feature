#include "NirsTrigger.h"
void NirsTrigger::update(double t){
	if (TriggerTime+0.1 <t) com<<"0"<<std::endl;
} 
void NirsTrigger::on(double t){
	TriggerTime=t;
	com<<"1"<<std::endl;
}