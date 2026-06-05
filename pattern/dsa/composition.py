class Engine:
    def __init__(self, name):
        self.name = name
        print(f"Engine {name} start")


class Car:
    def __init__(self, e_name, doors):
        self.engine = Engine(e_name)

    def __repr__(self):
        return f"Engine {self.engine.name} print"


car = Car("toyota", 2)
print(car)
