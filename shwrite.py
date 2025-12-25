with open(f'selections_history.txt', 'r') as shread:
    with open(f'sh.txt', 'w+') as shwrite:
        for line in shread:
            if "Length: 0" not in line:
                shwrite.write(line)
