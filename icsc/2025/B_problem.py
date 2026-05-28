def cake_calculator(flour, sugar):
    # Tính số bánh tối đa làm được dựa trên giới hạn nguyên liệu
    cake = min(flour // 100, sugar // 50)

    # Tính lượng nguyên liệu còn dư
    remaining_flour = flour - (cake * 100)
    remaining_sugar = sugar - (cake * 50)

    return [cake, remaining_flour, remaining_sugar]
