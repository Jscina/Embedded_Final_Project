from dataclasses import dataclass
from enum import Enum, auto
import time
import keyboard


class Light(Enum):
    RED = auto()
    YELLOW = auto()
    GREEN = auto()


@dataclass(slots=True)
class Stoplight:
    light: Light

    def switch(self):
        """Switches the light to the next color

        Returns:
            Light: The new light color
        """
        if self.light == Light.RED:
            self.light = Light.GREEN
        elif self.light == Light.GREEN:
            self.light = Light.YELLOW
        else:
            self.light = Light.RED
        print(f"Light switched to {self.light.name}")
        return self.light


@dataclass(slots=True)
class StoplightController:
    stoplights: list[Stoplight]
    current: int = 0

    def run(self):
        """Runs the stoplight controller"""
        new_light = self.stoplights[self.current].switch()
        if new_light == Light.RED:
            self.current = (self.current + 1) % len(self.stoplights)


@dataclass(slots=True)
class Street:
    name: str
    controller: StoplightController


@dataclass(slots=True)
class CarSensor:
    controller: StoplightController

    def detect_car(self):
        self.controller.run()


@dataclass(slots=True)
class CrosswalkButton:
    controller: StoplightController

    def press_button(self):
        time.sleep(5)  # delay
        self.controller.run()


class StoplightController:
    def __init__(self, stoplights):
        self.stoplights = stoplights

    def trigger_light_change(self):
        for stoplight in self.stoplights:
            stoplight


@dataclass(slots=True)
class Intersection:
    streets: list[Street]
    car_sensor: CarSensor
    crosswalk_button: CrosswalkButton

    def run(self):
        for street in self.streets:
            print(f"Running {street.name}")
            street.controller.run()
        # Simulate car detection and button press
        keyboard.add_hotkey("c", self.car_sensor.detect_car)
        keyboard.add_hotkey("b", self.crosswalk_button.press_button)
        keyboard.wait()


def setup_street(name: str, light: Light) -> Street:
    stoplight = Stoplight(light=light)
    controller = StoplightController(stoplights=[stoplight])
    return Street(name=name, controller=controller)


def main():
    # Initial street configuration
    main_street: Street = setup_street(
        name="Street 1",
        light=Light.GREEN,
    )
    side_street: Street = setup_street(
        name="Street 2",
        light=Light.RED,
    )

    crosswalk_button = CrosswalkButton(controller=main_street.controller)
    car_sensor = CarSensor(controller=side_street.controller)
    intersection = Intersection(
        streets=[
            main_street,
            side_street,
        ],
        crosswalk_button=crosswalk_button,
        car_sensor=car_sensor,
    )
    intersection.run()


if __name__ == "__main__":
    main()
