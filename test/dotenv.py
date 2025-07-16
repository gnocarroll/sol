import os


def read_dotenv(path: str) -> dict[str,str]:
    with open(path, "r") as f:
        return _read_dotenv(f)


def _read_dotenv(f) -> dict[str,str]:
    DOTENV_VARS = {}

    for line in f:
        k_v = get_key_value(line)

        if k_v is None:
            continue

        k, v = k_v

        DOTENV_VARS[k] = v

    return DOTENV_VARS

def get_key_value(line: str) -> tuple[str,str] | None:
    comment_idx = line.find("#")

    if comment_idx != -1:
        line = line[:comment_idx]

    line = line.strip()

    if len(line) == 0:
        return None
    
    eq_idx = line.find("=")

    if eq_idx == -1:
        return None

    key_text = line[:eq_idx].strip()

    value_text = line[eq_idx + 1:].strip()

    ret = lambda: (key_text, value_text)

    if len(value_text) == 0:
        return ret()
    
    if value_text[0] in ("'", "\""):
        value_text = value_text[1:]
    
    if len(value_text) == 0:
        return ret()

    if value_text[-1] in ("'", "\""):
        value_text = value_text[:-1]

    return ret()
    

    