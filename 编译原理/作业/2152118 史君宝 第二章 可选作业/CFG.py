def parse_S(input_string):
    if input_string == '':
        return True
    elif len(input_string) >= 2 and input_string[0] == 'a' and input_string[-1] == 'b':
        return parse_S(input_string[1:-1])
    else:
        return False
    

print("程序开始，输入q结束程序")
input_string = input("请输入一个字符串：")
while input_string != 'q':
    result = parse_S(input_string)
    if result:
        print("Yes")
    else:
        print("No")  
    input_string = input("请输入一个字符串：")
print("程序结束")