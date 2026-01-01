"""
This Python code simulates a resource management system within a virtual world.
The world consists of workers, places to store resources,
and transitions that control the flow of resources.
"""

import random
from abc import ABC, abstractmethod


class Resource(ABC):
    """
    An abstract base class representing a generic resource. It is the
    parent class for specific resource types like workers, food, and products.
    """

    def __init__(self):
        pass


class Place(ABC):
    """
    An abstract base class representing a location where resources can be
    stored and managed. Subclasses like Barrack, Barn, and Warehouse implement
    specific places for different types of resources.
    """

    def __init__(self):
        self._resource = []

    @abstractmethod
    def resource_in_place(self):
        pass

    @abstractmethod
    def add(self, resource: Resource):
        pass

    @abstractmethod
    def send(self):
        pass


class Transition(ABC):
    """
    An abstract base class representing actions that can be performed in the
    world, such as creating new resources or processing existing ones.
    Subclasses like Home, Factory, FoodCourt, and Field implement specific
    actions.
    """

    def __init__(self):
        self._to_barrack = None
        self._from_barrack = None
        self._place = None

    @abstractmethod
    def action(self):
        pass

    @abstractmethod
    def change_barrack(self, barrack1, barrack2):
        pass

    @abstractmethod
    def change_place(self, place: Place):
        pass


class Worker(Resource):
    """
    A class representing a worker resource with health points. Workers can be
    created, their health managed, and they can be sent to different places.
    """

    worker_number = 1

    def __init__(self):
        super().__init__()
        self.hp = 100
        self.worker_number = Worker.worker_number
        Worker.worker_number += 1

    def end_life(self, barrack):
        barrack.send(self)


class Food(Resource):
    """
    A class representing a type of resource food with a quality attribute.
    It's stored in the Barn and can be consumed by workers.
    """

    def __init__(self, quality):
        super().__init__()
        self._quality = quality


class Product(Resource):
    """
    A class representing a type of resource product
    that can be produced and stored in the Warehouse.
    """

    def __init__(self):
        super().__init__()
        pass


class Barrack(Place):
    """
    A place where workers are stored and managed.
    Workers can be added and sent from the Barrack
    """

    def __init__(self):
        super().__init__()
        self._resource = []

    def add(self, worker):
        if worker.hp > 0:
            self._resource.append(worker)
        else:
            print(f"WORKER {worker.worker_number} died.")

    def send(self):
        if self.resource_in_place():
            return self._resource.pop(0)
        else:
            return None

    def resource_in_place(self):
        return len(self._resource) > 0


class Barn(Place):
    """
    A place for storing food.
    Food can be added and sent from the Barn.
    """

    def __init__(self):
        super().__init__()
        self._resource = []

    def add(self, food):
        self._resource.append(food)

    def send(self):
        if self.resource_in_place():
            return self._resource.pop(0)
        else:
            return None

    def resource_in_place(self):
        return len(self._resource) > 0


class Warehouse(Place):
    """
    A place for storing product.
    Products can be added and sent from the Warehouse.
    """

    def __init__(self):
        super().__init__()
        self._resource = []

    def add(self, product):
        self._resource.append(product)

    def send(self):
        if self.resource_in_place():
            return self._resource.pop()
        else:
            return None

    def resource_in_place(self):
        return len(self._resource) > 0


class Home(Transition):
    """
    A transition representing a worker's home where they can rest,
    regain health, and potentially create new workers.
    """

    def __init__(self):
        super().__init__()
        self._to_barrack = None
        self._from_barrack = None
        self._place = None

    def _sleep(self):
        if self._from_barrack and self._place:
            worker = self._from_barrack.send()
            product = self._place.send()
            print(f"WORKER {worker.worker_number} arrived at the HOME.")
            if worker and product:
                if worker.hp < 100:
                    worker.hp += random.randint(1, 5)
                    self._place.send()
                    print(f"WORKER's HP after SLEEPING: {worker.hp}.")
                    if worker.hp >= 100:
                        worker.hp = 100
                        print("WORKER's HP capped at 100.")
                elif worker.hp == 100:
                    print(f"WORKER {worker.worker_number} HP already 100.")
            elif not worker:
                print("No WORKER's available in the BARRACK.")
            elif not product:
                print("No PRODUCT available in the WAREHOUSE.")
        else:
            print("HOME is not properly connected to a BARRACK or WAREHOUSE.")
        self._to_barrack.add(worker)
        print("WORKER returned to the BARRACK.")

    def _multiply(self, worker1, worker2, product):
        world = World()
        if isinstance(worker1, Worker) and isinstance(worker2, Worker):
            if isinstance(product, Product):
                if self._to_barrack and self._from_barrack and self._place:
                    if worker1.hp > 0 and worker2.hp > 0:
                        if self._place.resource_in_place():
                            print("WORKER is MULTIPLYING.")
                            self._to_barrack.add(worker1)
                            self._to_barrack.add(worker2)
                            self._place.send()
                            print("WORKER created, returned to the BARRACK.")
                            new_worker = world.create_worker()
                            self._to_barrack.add(new_worker)
                            return new_worker
                        else:
                            print("No PRODUCT available.")
                    else:
                        print("WORKER's must have positive health points.")
                else:
                    print("HOME is not properly connected.")
            else:
                print("Invalid input. You must provide a PRODUCT.")
        else:
            print("Invalid input. You must provide two WORKER's")

    def action(self):
        if len(self._from_barrack._resource) == 2:
            if self._place.resource_in_place():
                worker1 = self._from_barrack.send()
                worker2 = self._from_barrack.send()
                product = Product()
                self._multiply(worker1, worker2, product)
        else:
            self._sleep()

    def change_barrack(self, barrack1, barrack2):
        self._from_barrack = barrack1
        self._to_barrack = barrack2

    def change_place(self, warehouse):
        self._place = warehouse


class FoodCourt(Transition):
    """
    A transition where workers can consume food to regain health.
    """

    def __init__(self):
        super().__init__()
        self._to_barrack = None
        self._from_barrack = None
        self._place = None

    def action(self):
        if self._from_barrack and self._place:
            worker = self._from_barrack.send()
            food = self._place.send()
            print(f"WORKER {worker.worker_number} arrived at the FOOD COURT.")
            if worker and food:
                if worker.hp < 100:
                    worker.hp += food._quality
                    self._place.send()
                    print(f"WORKER's HP after EATING: {worker.hp}.")
                    if worker.hp >= 100:
                        worker.hp = 100
                        print("WORKER's HP capped at 100.")
                elif worker.hp == 100:
                    print(f"WORKER {worker.worker_number} HP already 100.")
            elif not worker:
                print("No WORKER's available in the BARRACK.")
            elif not food:
                print("No FOOD available in the BARN.")
        else:
            print("FOOD COURT is not properly connected to a BARRACK or BARN.")
        self._to_barrack.add(worker)
        print("WORKER returned to the BARRACK.")

    def change_barrack(self, barrack1, barrack2):
        self._from_barrack = barrack1
        self._to_barrack = barrack2

    def change_place(self, barn):
        self._place = barn


class Factory(Transition):
    """
    A transition where workers can produce products
    but may also lose health in the process.
    """

    def __init__(self):
        super().__init__()
        self._to_barrack = None
        self._from_barrack = None
        self._place = None

    def action(self):
        if self._from_barrack:
            worker = self._from_barrack.send()
            if worker:
                print(f"WORKER {worker.worker_number} arrived at the FACTORY.")
                product = Product()
                print("FACTORY produced a PRODUCT.")
                self._lower_hp(worker)
                print(f"WORKER's HP after working: {worker.hp}.")
                if worker.hp > 0:
                    self._to_barrack.add(worker)
                    print("WORKER returned to the BARRACK.")
                    return product
                else:
                    print(f"WORKER {worker.worker_number} died.")
            else:
                print("No WORKER available in the BARRACK.")
        else:
            print("FACTORY is not connected to a BARRACK.")

    def _lower_hp(self, worker):
        hp_reduction = random.randint(20, 40)
        worker.hp -= hp_reduction
        if worker.hp < 0:
            worker.hp = 0

    def send_product(self, product):
        if self._place:
            self._place.add(product)
            print("PRODUCT sent to the WAREHOUSE.")

    def change_barrack(self, barrack1, barrack2):
        self._from_barrack = barrack1
        self._to_barrack = barrack2

    def change_place(self, warehouse):
        self._place = warehouse


class Field(Transition):
    """
    A transition where workers can gather food but might
    encounter accidents that reduce their health.
    """

    def __init__(self):
        super().__init__()
        self._to_barrack = None
        self._from_barrack = None
        self._place = None

    def action(self):
        accident = random.randint(1, 2)
        if self._from_barrack:
            worker = self._from_barrack.send()
            if worker:
                print(f"WORKER {worker.worker_number} arrived at the FIELD.")
                food = Food(random.randint(1, 10))
                print("FIELD produced FOOD.")
                if accident == 1:
                    self._lower_hp(worker)
                    print(f"WORKER's HP after accident: {worker.hp}")
                if worker.hp > 0:
                    self._to_barrack.add(worker)
                    print("WORKER returned to the BARRACK.")
                    return food
                else:
                    print(f"WORKER {worker.worker_number} died.")
            else:
                print("No WORKER's available in the BARRACK.")
        else:
            print("FIELD is not connected to a BARRACK.")

    def _lower_hp(self, worker):
        hp_reduction = random.randint(60, 80)
        worker.hp -= hp_reduction
        if worker.hp < 0:
            worker.hp = 0

    def send_food(self, food):
        if self._place:
            self._place.add(food)
            print("FOOD sent to the BARN.")

    def change_barrack(self, barrack1, barrack2):
        self._from_barrack = barrack1
        self._to_barrack = barrack2

    def change_place(self, barn):
        self._place = barn


class World:
    """
    A class that manages the creation and interaction of various entities
    within the virtual world. It provides methods to create
    entities and initiate the simulation.
    """

    def __init__(self):
        self._barracks = []
        self._homes = []
        self._food_courts = []
        self._factories = []
        self._warehouses = []
        self._fields = []
        self._barns = []

    def create_worker(self):
        worker = Worker()
        print(f"Created WORKER {worker.worker_number}")
        return worker

    def create_barrack(self):
        barrack = Barrack()
        self._barracks.append(barrack)
        return barrack

    def create_home(self):
        home = Home()
        self._homes.append(home)
        return home

    def create_food_court(self):
        food_court = FoodCourt()
        self._food_courts.append(food_court)
        return food_court

    def create_warehouse(self):
        warehouse = Warehouse()
        self._warehouses.append(warehouse)
        return warehouse

    def create_factory(self):
        factory = Factory()
        self._factories.append(factory)
        return factory

    def create_barn(self):
        barn = Barn()
        self._barns.append(barn)
        return barn

    def create_field(self):
        field = Field()
        self._fields.append(field)
        return field

    def start_system(self):

        if not self._barracks:
            barrack = self.create_barrack()
        else:
            barrack = self._barracks[0]

        if not self._barns:
            barn = self.create_barn()
        else:
            barn = self._barns[0]

        if not self._warehouses:
            warehouse = self.create_warehouse()
        else:
            warehouse = self._warehouses[0]

        if not self._homes:
            home = self.create_home()
            home.change_barrack(barrack, barrack)
            home.change_place(warehouse)
        else:
            home = self._homes[0]

        if not self._factories:
            factory = self.create_factory()
            factory.change_barrack(barrack, barrack)
            factory.change_place(warehouse)
        else:
            factory = self._factories[0]

        if not self._food_courts:
            food_court = self.create_food_court()
            food_court.change_barrack(barrack, barrack)
            food_court.change_place(barn)
        else:
            food_court = self._food_courts[0]

        if not self._fields:
            field = self.create_field()
            field.change_barrack(barrack, barrack)
            field.change_place(barn)
        else:
            field = self._fields[0]

        for _ in range(2):
            worker = self.create_worker()
            self._barracks[0].add(worker)

        while len(self._barracks[0]._resource) > 0:

            if worker.hp > 0:
                home.action()

            if worker.hp > 0:
                print("#####################################################")

            for worker in self._barracks[0]._resource:
                if worker.hp > 0:
                    product = factory.action()
                    if worker.hp > 0:
                        factory.send_product(product)
                        print("Products in the warehouse:" +
                              str(len(factory._place._resource)))
                break

            if worker.hp > 0:
                print("#####################################################")

            if worker.hp > 0:
                food_court.action()

            if worker.hp > 0:
                print("#####################################################")

            for worker in self._barracks[0]._resource:
                if worker.hp > 0:
                    food = field.action()
                    if worker.hp > 0:
                        field.send_food(food)
                        print("Food in the Barn: " +
                              str(len(field._place._resource)))
                break

            if worker.hp > 0:
                print("#####################################################")

            for worker in self._barracks[0]._resource:
                if worker.hp <= 0:
                    worker.end_life(barrack)

            if not self._barracks[0]._resource:
                break


if __name__ == "__main__":
    world = World()
    world.start_system()
