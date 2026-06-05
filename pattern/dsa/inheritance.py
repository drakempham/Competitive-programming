from abc import ABC, abstractmethod


class Vehicle(ABC):
    def __init__(self, brand):
        self.brand = brand

    @abstractmethod
    def start(self):
        print(f"{self.brand} is init")


class Car(Vehicle):
    def __init__(self, brand, doors):
        super().__init__(brand)
        self.doors = doors

    def start(self):
        super().start()

    def open_trunks(self):
        super().start()
        print("Open trunks")


car = Car("toyota", 2)
car.start()
car.open_trunks()
print("Brand: ", car.brand)
print("Doors: ", car.doors)
