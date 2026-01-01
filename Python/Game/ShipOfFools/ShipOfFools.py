import random


class Die:

    def __init__(self):
        self._value = self.roll()

    def get_value(self):
        return self._value

    def roll(self):
        self._value = random.randint(1, 6)


class DiceCup:

    def __init__(self, num_dice):
        self._dice = [(Die(), False) for _ in range(num_dice)]

    def die_value(self, index):
        return self._dice[index][0].get_value()

    def bank(self, index):
        self._dice[index] = (self._dice[index][0], True)

    def is_banked(self, index):
        return self._dice[index][1]

    def release(self, index):
        self._dice[index] = (self._dice[index][0], False)

    def release_all(self):
        for index in range(len(self._dice)):
            self._dice[index] = (self._dice[index][0], False)

    def roll(self):
        rolled_values = []
        for index in range(len(self._dice)):
            if self._dice[index][1] is False:
                self._dice[index][0].roll()
                rolled_values.append(self.die_value(index))
            else:
                rolled_values.append("Banked")

        print(rolled_values)
        return rolled_values


class Player:

    def __init__(self, name):
        self._name = name
        self._score = 0

    def current_score(self):
        return self._score

    def get_name(self):
        return self._name

    def reset_score(self):
        self._score = 0

    def play_turn(self, game):
        self._score += game.turn()


class PlayRoom:

    def __init__(self):
        self._game = None
        self._players = []
        self._winner = None

    def set_game(self, game):
        self._game = game

    def add_player(self, player):
        self._players.append(player)

    def reset_scores(self):
        for player in self._players:
            player.reset_score()

    def play_round(self):
        for player in self._players:
            player.play_turn(self._game)

    def game_finished(self):
        for player in self._players:
            if player.current_score() >= self._game.winning_score:
                self._winner = player
                return True
        return False

    def print_scores(self):
        for player in self._players:
            print(f"{player.get_name()}: {player.current_score()}")

    def print_winner(self):
        if self._winner:
            print(f"The winner is {self._winner.get_name()}!")
        else:
            print("No winner yet.")


class ShipOfFoolsGame:

    def __init__(self):
        self._num_of_dice = 5
        self._cup = DiceCup(self._num_of_dice)
        self.winning_score = 50

    def _cup_has(self, value):
        for index in range(self._num_of_dice):
            if value == self._cup.die_value(index):
                return True

    def _cup_bank(self, value):
        for index in range(self._num_of_dice):
            if value == self._cup.die_value(index):
                self._cup.bank(index)
                break

    def turn(self):
        has_ship = False
        has_captain = False
        has_mate = False
        crew = 0

        for _ in range(3):
            self._cup.roll()

            if has_ship is False and self._cup_has(6) is True:
                self._cup_bank(6)
                has_ship = True

            if has_ship is True and has_captain is False and self._cup_has(5) is True:
                self._cup_bank(5)
                has_captain = True

            if has_ship is True and has_captain is True and has_mate is False and self._cup_has(4) is True:
                self._cup_bank(4)
                has_mate = True

            for value in [6, 5, 4]:
                if has_ship is True and has_captain is True and has_mate is True and self._cup_has(value) is True:
                    for index in range(self._num_of_dice):
                        if self._cup.is_banked(index) is False and self._cup.die_value(index) > 3:
                            self._cup.bank(index)

        if has_ship is True and has_captain is True and has_mate is True:
            crew = (self._cup.die_value(0) + self._cup.die_value(1) + self._cup.die_value(2) + self._cup.die_value(3) + self._cup.die_value(4)) - 15

        print("Crew Score:", crew)
        self._cup.release_all()
        return crew


if __name__ == "__main__":

    room = PlayRoom()
    room.set_game(ShipOfFoolsGame())
    room.add_player(Player("Nora"))
    room.add_player(Player("Selman"))
    room.reset_scores()
    while not room.game_finished():
        room.play_round()
        room.print_scores()
    room.print_winner()
