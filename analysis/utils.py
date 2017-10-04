def isAny(expression):
    try:
        if expression:
            return True
    except ValueError:
        if any(expression):
            return True
    return False
