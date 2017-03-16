#include "Runner.h"
#include <iostream>



Runner::Runner()
{
}


Runner::~Runner()
{
}

void Runner::RegisterHandlers() {
	entity_->GetParent()->BroadcastEvent(Events::RunnerCreated{ entity_ });
}
