import random
import json
import orjson
import msgspec


def print_progress(current, total):
    percent = float(current) * 100 / total
    if current == 0 or current == total or current % max(1, (total // 20)) == 0:
        print(f'\rProgress: {percent:.1f}% ({current}/{total})', end='', flush=True)
    if current == total:
        print()


def random_string(max_length=10):
    # 0.1% chans att få lång sträng
    if random.random() < 0.001:
        max_length = 1000

    length = random.randint(0, max_length)
    if length == 0:
        return ""

    # teckenuppsättningar
    let_num = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    let_num_spec = let_num + "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
    unicode = (
        "äöüå€∑日本語汉字한글🙂🚀🌍αβγΩπΔЖдШћ"
        "–—•«»¿¡©®™✓"
    )
    control = [chr(i) for i in range(1, 32)]
    uncommon = [
        'Quote: "inside"',
        'Backslash: \\path\\to\\file',
        'Escapes: \n\r\t\b\f',
        'Unicode escape: \u0061\u0062\u0063',
        '\u0000',
        '🔥' * 10
    ]

    # välj kategori
    case = random.randint(1, 4)
    if case == 1:
        # bara bokstäver/siffror
        return ''.join(random.choice(let_num) for _ in range(length))
    if case == 2:
        # med punktuation
        return ''.join(random.choice(let_num_spec) for _ in range(length))
    if case == 3:
        # unicode + emoji
        return ''.join(random.choice(unicode) for _ in range(length))
    # kontrolltecken (50% chans) eller specialsträng
    if random.random() < 0.5:
        # max 5 kontrolltecken
        k = random.randint(1, min(5, length))
        return ''.join(random.choice(control) for _ in range(k))
    return random.choice(uncommon)
        

def random_number():
    case = random.randint(1, 5)

    if case == 1:
        # Heltal mellan -100 och 100
        return random.randint(-100, 100)
    if case == 2:
        # Heltal mellan -1 000 000 och 1 000 000
        return random.randint(-10**6, 10**6)
    if case == 3:
        # 32-bitars heltal
        return random.randint(-(2**31), 2**31 - 1)
    if case == 4:
        # Flyttal mellan -100.0 och 100.0
        return random.uniform(-100, 100)

    # Extrema värden: mycket stora/små floats eller stora heltal
    return random.choice([
        1e+30,
        -1e+30,
        0.0,
        1e-30,
        random.randint(2**63, 2**70),
        random.randint(-(2**70), -(2**63)),
        1e+308 * random.random()
    ])


def random_value(depth=0, max_depth=4):
    # välj typer beroende på djup
    if depth >= max_depth:
        types = ['number', 'string', 'scalar']
    else:
        types = ['number', 'string', 'scalar', 'list', 'dict']
    
    case = random.choice(types)
    
    if case == 'number':
        # tal
        return random_number()
    if case == 'string':
        # textsträng
        return random_string()
    if case == 'scalar':
        # bool eller None
        return random.choice([True, False, None])
    if case == 'list':
        # lista med värden
        size = random.randint(0, 5)
        return [random_value(depth+1, max_depth) for _ in range(size)]
    
    # dict med nyckel: värde
    size = random.randint(0, 5)
    return {random_string(5): random_value(depth+1, max_depth) for _ in range(size)}


def random_data_generator():
    kinds = ['object', 'nested', 'list', 'value']
    while True:
        kind = random.choice(kinds)
        if kind == 'object':
            # objekt med 3 fält
            yield {f'field_{i}': random_value() for i in range(1, 4)}
        elif kind == 'nested':
            # nested + lista
            yield {
                'nested': {
                    'l1': random_value(),
                    'l2': random_value()
                },
                'array': [random_value() for _ in range(random.randint(0, 3))]
            }
        elif kind == 'list':
            # lista med värden
            yield [random_value() for _ in range(random.randint(0, 5))]
        else:
            # enkelt värde
            yield random_value()



def is_equal(left, right, *, float_tol=1e-6):
    # Flyttal: jämför med tolerans
    if isinstance(left, float) and isinstance(right, float):
        if left == right:
            return True
        diff = abs(left - right)
        return diff <= max(float_tol * max(abs(left), abs(right)), float_tol)

    # Annars vanlig likhet
    else:
        return left == right


def main():
    random.seed(9001)
    data_generator = random_data_generator()
    exeptions = []
    mismatches = []
    total_tests = 1000
    for i in range(total_tests):
        print_progress(i, total_tests)
        data = next(data_generator)
        try:
            output_json = json.dumps(data, indent=None, separators=(',', ':'), ensure_ascii=False).encode('utf8')
            output_orjson = orjson.dumps(data)
            output_mesgspec = msgspec.json.encode(data)
        except Exception as exception:
            exeptions += [(exception, data)]
        else:
            if not output_json == output_orjson == output_mesgspec:
                try:
                    json_obj = json.loads(output_json)
                    orjson_obj = json.loads(output_orjson)
                    msgspec_obj = json.loads(output_mesgspec)
                    
                    semantic_equal = (
                        is_equal(json_obj, orjson_obj) and 
                        is_equal(json_obj, msgspec_obj) and 
                        is_equal(orjson_obj, msgspec_obj)
                    )
                    
                    print(f"Raw outputs differ. Semantic equality: {semantic_equal}")
                    print(f"JSON: {output_json}")
                    print(f"ORJSON: {output_orjson}")
                    print(f"MSGSPEC: {output_mesgspec}")
                    print("")
                except Exception as e:
                    print(f"Error comparing outputs: {e}")
                
                if semantic_equal:
                    mismatches += [data]
    
    if exeptions:
        exception_types = {}
        for ex, data in exeptions:
            exception_type = type(ex).__name__
            if exception_type not in exception_types:
                exception_types[exception_type] = []
            exception_types[exception_type].append((str(ex), data))
        
        for exception_type, instances in exception_types.items():
            print(f"\n{exception_type} ({len(instances)} occurrences):")
            for i, (exception_msg, data) in enumerate(instances[:len(instances)]):
                print(f"  {i+1}. {exception_msg}")
                print(f"     Data: {str(data)[:100]}{'...' if len(str(data)) > 100 else ''}")

    print('\n')            
    print_progress(total_tests, total_tests)
    print(f'\n{len(exeptions)} exceptions and {len(mismatches)} mismatches found\n')


if __name__ == '__main__':
    main()
