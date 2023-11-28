import time
import keyboard


class LED:
    def __init__(self, color, street):
        self.color = color
        self.state = False
        self.street = street

    def turn_on(self):
        self.state = True

    def turn_off(self):
        self.state = False

    def __str__(self):
        return f"{self.street} St {self.color} light is {'on' if self.state else 'off'}"


class Button:
    def __init__(self):
        self.state = False

    def press(self):
        self.state = True

    def release(self):
        self.state = False


class Sensor:
    def __init__(self):
        self.state = False

    def detect(self):
        self.state = True

    def clear(self):
        self.state = False


class TrafficController:
    def __init__(self):
        # Initialize LEDs
        self.main_st_red = LED("red", "Main")
        self.main_st_yellow = LED("yellow", "Main")
        self.main_st_green = LED("green", "Main")
        self.side_st_red = LED("red", "Side")
        self.side_st_yellow = LED("yellow", "Side")
        self.side_st_green = LED("green", "Side")

        # Initialize button and sensor
        self.crosswalk_button = Button()
        self.car_sensor = Sensor()

    def toggle_crosswalk(self, e=None):
        self.crosswalk_button.press()
        time.sleep(0.5)
        self.crosswalk_button.release()

    def detect_car(self, e=None):
        self.car_sensor.detect()
        time.sleep(0.5)
        self.car_sensor.clear()

    def run(self):
        keyboard.on_press_key("c", self.toggle_crosswalk)
        keyboard.on_press_key("s", self.detect_car)
        while True:
            if self.crosswalk_button.state or self.car_sensor.state:
                # Change Main St to Yellow
                self.main_st_green.turn_off()
                self.main_st_yellow.turn_on()
                print(self.main_st_yellow, end="\n" + "-" * 20 + "\n\n")
                time.sleep(2)  # Delay for Yellow

                # Change Main St to Red and Side St to Green
                self.main_st_yellow.turn_off()
                self.main_st_red.turn_on()
                print(self.main_st_red)
                self.side_st_red.turn_off()
                self.side_st_green.turn_on()
                print(self.side_st_green, end="\n" + "-" * 20 + "\n\n")
                time.sleep(5)  # Delay for Red/Green

                # Change Side St to Yellow before turning Red
                self.side_st_green.turn_off()
                self.side_st_yellow.turn_on()
                print(self.side_st_yellow, end="\n" + "-" * 20 + "\n\n")
                time.sleep(2)  # Delay for Yellow

            else:
                self.main_st_red.turn_off()
                self.main_st_green.turn_on()
                print(self.main_st_green)
                self.side_st_yellow.turn_off()
                self.side_st_red.turn_on()
                print(self.side_st_red, end="\n" + "-" * 20 + "\n\n")
                time.sleep(0.1)


def main():
    controller = TrafficController()
    controller.run()


if __name__ == "__main__":
    main()
