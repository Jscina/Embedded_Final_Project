from dataclasses import dataclass
from enum import Enum, auto
import time


class Light(Enum):
    RED = auto()
    YELLOW = auto()
    GREEN = auto()


@dataclass(slots=True)
class Stoplight:
    light: Light
    duration: dict[Light, int]

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
class Intersection:
    streets: list[Street]

    def run(self):
        while True:
            max_duration = 0
            for street in self.streets:
                print(f"Changing lights for {street.name}")
                street.controller.run()
                current_duration = street.controller.stoplights[0].duration[
                    street.controller.stoplights[0].light
                ]
                if current_duration > max_duration:
                    max_duration = current_duration
            time.sleep(max_duration)
            print("#" * 20)


def setup_street(name: str, light: Light, duration: dict[Light, int]) -> Street:
    stoplight = Stoplight(light=light, duration=duration)
    controller = StoplightController(stoplights=[stoplight])
    return Street(name=name, controller=controller)


def main():
    duration = {Light.RED: 5, Light.YELLOW: 3, Light.GREEN: 5}
    # Initial street configuration
    main_street = setup_street(name="Street 1", light=Light.GREEN, duration=duration)
    second_street = setup_street(name="Street 2", light=Light.GREEN, duration=duration)
    third_street = setup_street(name="Street 3", light=Light.RED, duration=duration)
    intersection = Intersection(streets=[main_street, second_street, third_street])
    intersection.run()


if __name__ == "__main__":
    main()
