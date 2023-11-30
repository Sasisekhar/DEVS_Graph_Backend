#include <cadmium/core/logger/csv.hpp>
#include <cadmium/core/simulation/root_coordinator.hpp>
#include <cadmium/lib/iestream.hpp>
#include <limits>

#include "Lobby.h"
#include "Bar.h"
#include "Table.h"

using namespace std;
using namespace cadmium::lib;

namespace cadmium::iot {

struct Restaurant: public Coupled {
	Port<int> in2;
	Port<int> in3;
	Port<int> out5;
	Restaurant(const std::string &id): Coupled(id){
		auto bar = addComponent<Bar>("bar");
		auto table = addComponent<Table>("table");
		in2 = addInPort<int>("in2");
		in3 = addInPort<int>("in3");
		out5 = addOutPort<int>("out5");
		addCoupling(in2, bar->in4);
		addCoupling(in3, table->in5);
		addCoupling(bar->out3, out5);
		addCoupling(table->out4, out5);
	}
};
struct top_model: public Coupled {
	Port<int> output;
	top_model(const std::string &id): Coupled(id){
		auto lobby = addComponent<Lobby>("lobby");
		auto restaurant = addComponent<Restaurant>("restaurant");
		auto filePath = "input0.txt";
		auto iestream = addComponent<IEStream<int>>("iestream", filePath);
		output = addOutPort<int>("output");
		addCoupling(lobby->out1, restaurant->in2);
		addCoupling(lobby->out2, restaurant->in3);
		addCoupling(iestream->out, lobby->in1);
		addCoupling(restaurant->out5, output);
	}
};


}

using namespace cadmium::iot;

int main() {
  
    auto model = make_shared<top_model>("top");
    auto rootCoordinator = cadmium::RootCoordinator(model);
    auto logger = make_shared<cadmium::CSVLogger>("log.csv", ";");
    rootCoordinator.setLogger(logger);
    rootCoordinator.start();
    rootCoordinator.simulate(10.0);
    rootCoordinator.stop();
    return 0;
}
