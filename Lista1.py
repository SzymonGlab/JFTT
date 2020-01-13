def initialize_q(pattern):
    q_function = {}
    for letter in range(len(pattern) + 1):
        if letter not in q_function:
            q_function[letter] = {}
            for character in pattern:
                if character not in q_function[letter]:
                    q_function[letter][character] = 0
    return q_function

# O(m^3|Σ|) - preprocessing (m-pattern length)
def compute_transition_function(alphabet, pattern):
    q_function = initialize_q(pattern)
    m = len(pattern)
    for q in range(m + 1):
        for letter in alphabet:
            k = min(m, q + 1)
            pk = pattern[0:k]
            pq = pattern[0:q] + letter
            while not (pq.endswith(pk)) and k != 0:
                k -= 1
                pk = pattern[0:k]
                pq = pattern[0:q] + letter
            q_function[q][letter] = k
    return q_function

# Θ(n) - matching time (n-text length)
def finite_automaton_matcher(text, function, m):
    n = len(text)
    q = 0
    for i in range(n):
        q = function[q][text[i]]
        if q == m:
            print("Wzorzec występuje z przesunięciem " + str(i - m + 1))


# Θ(m|Σ|) - precomputing time 
def compute_prefix_function(pattern):
    m = len(pattern)
    pi = {1: 0}
    k = 0
    for q in range(2, m + 1):
        while k > 0 and pattern[k + 1] != pattern[q]:
            k = pi[k]
        if pattern[k + 1] == pattern[q]:
            k += 1
        pi[q] = k
    return pi

# Θ(n) - matching time
def kmp_matcher(text, pattern):
    n = len(text)
    m = len(pattern)
    pi = compute_prefix_function(pattern)
    q = 0
    for i in range(1, n + 1):
        while q > 0 and pattern[q + 1] != text[i]:  
            q = pi[q]
        if pattern[q + 1] == text[i]:
            q += 1
        if q == m:
            print('Wzorzec występuje z przesunięciem ', i - m)
            q = pi[q]


if __name__ == "__main__":
    choice = input("Wybierz algorytm:\n 1. Finite automaton matcher\n 2. KMP matcher\n")
    if(choice == '1'):
        letters = input("Wprowadź alfabet\n")
        a = list(letters)
        t = input('Wprowadź tekst\n')
        while True:
            p = input("Wprowadź wzorzec\n")
            finite_automaton_matcher(t, compute_transition_function(a, p), len(p))    
    elif(choice == '2'):
        t_tmp = input('Wprowadź tekst\n')
        t = {i + 1: t_tmp[i] for i in range(0, len(t_tmp))}
        while True:
            p_tmp = input('Wprowadź wzorzec\n')
            p = {i + 1: p_tmp[i] for i in range(0, len(p_tmp))}
            kmp_matcher(t, p)
    else:
        print("Wrong choice, choose between 1 and 2.")
    
    #🙂🙃😉🙁
    #😉😉🙂🙃😉😉🙂😉🙃😉🙃😉🙃🙂😉🙃😉😉🙂😉🙂🙂🙃🙂🙂🙂🙂😉😉🙃😉🙃😉🙃🙂🙃🙃😉🙃🙃🙃😉🙃😉🙃😉🙂🙂🙃🙂🙃🙃🙂😉🙂🙃🙂😉🙂🙃😉🙂🙂🙂

    #🙁
    #😉😉
    #😉🙂😉🙂
    #🙃🙂🙂🙂🙂😉😉🙃🙃
    #😉🙂🙂🙂
    #🙃😉🙃