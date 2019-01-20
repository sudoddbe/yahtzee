from yahtzee_solver import Solver

def test_create():
    s = Solver()
    assert(s != None)
    print len(s.game_map)
    print len(s.game_map[0])
    print len(s.game_map[1])
    print len(s.game_map[2])
    print len(s.game_map[0][0])

def test_select_category():
    s = Solver()
    for cat in s.scorecard.categories[0:-2]:
        cat.fill(score = 0)
    c = s.select_category_greedy(rolls = (1,2,3,4,5))
    assert(c == s.scorecard.categories[-1].name)

    s = Solver()
    c = s.select_category_greedy(rolls = (1,2,3,4,5))
    assert(c == 'small_straight')

if __name__ == "__main__":
    test_create()
    test_select_category()
