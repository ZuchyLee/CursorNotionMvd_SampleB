#include <Automotive/VehicleFactory.hpp>

#include <iomanip>
#include <iostream>

int main() {
    std::cout << "--- ProjectB Automotive Integration Demo ---\n";

    auto car = Automotive::VehicleFactory::createVehicle(
        Automotive::VehicleFactory::CarType::Sedan_EV
    );
    if (!car) {
        std::cerr << "Failed to create vehicle.\n";
        return 1;
    }

    std::cout << "Model: " << car->getModelName() << "\n";

    // 正常操作流程：先上電再控制
    car->setPower(true);

    // ❌ 示範用的規範違規（AutomotiveModule_Review_Guidelines.md 3.2 浮點數範圍）
    // setThrottle(position) 的合法範圍是 0.0 ~ 1.0
    car->setThrottle(1.2f);

    car->setSteering(-0.2f);

    const auto state = car->getState();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "speed=" << state.speed << " rpm=" << state.rpm << "\n";

    car->setBrake(1.0f);
    car->setPower(false);
    return 0;
}

