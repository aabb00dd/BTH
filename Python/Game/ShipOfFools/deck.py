class card:

    def __init__(self, suit, value):
        self.suit = suit
        self._value = value

    def to_string(self):
        return self.suit + str(self._value)


class Hand():

    def __init__(self):
        self._cards = []


class Deck():

    def __init__(self):
        self._stack = []
        for s in ["H", "S", "R", "K"]:
            for v in range(1, 14):
                self._stack.append(card(s, v))

    def to_string(self):
        ret_s = ""
        for c in self._stack:
            ret_s = ret_s + c.to_string() + " "
        return ret_s


if __name__ == "__main__":
    # Card = card("H", 1) #skapade ett objekt
    # print(card.to_string())
    deck = Deck()
    print(deck.to_string())
