from math import *
import functools

log_infinity = 9999

def delta_BKZ(b):
    """ The root hermite factor delta of BKZ-b
    """
    return ((pi*b)**(1./b) * b / (2*pi*exp(1)))**(1./(2.*b-2.))


def svp_plausible(b):
    """ log_2 of best plausible Quantum Cost of SVP in dimension b
    """
    return b *log(sqrt(4./3))/log(2)   # .2075 * b


def svp_quantum(b):
    """ log_2 of best plausible Quantum Cost of SVP in dimension b
    """
    return b *log(sqrt(13./9))/log(2)   # .265 * b  [Laarhoven Thesis]


def svp_classical(b):
    """ log_2 of best known Quantum Cost of SVP in dimension b
    """
    return b *log(sqrt(3./2))/log(2)    # .292 * b [Becker Ducas Laarhoven Gama]


def nvec_sieve(b):
    """ Number of short vectors outputted by a sieve step of blocksize b
    """
    return b *log(sqrt(4./3))/log(2)    # .2075 * b


## Adding Memoization to this really slow function
# @functools.lru_cache(maxsize=2**20)
def construct_BKZ_shape(q, nq, n1, b):
    """ Simulate the (log) shape of a basis after the reduction of
        a [q ... q, 1 ... 1] shape after BKZ-b reduction (nq many q's, n1 many 1's)
        This is implemented by constructing a longer shape and looking
        for the subshape with the right volume. Also outputs the index of the
        first vector <q, and the last >q.

        # Note: this implentation takes O(n). It is possible to output
        # a compressed description of the shape in time O(1), but it is much
        # more prone to making mistakes

    """
    d = nq+n1
    if b==0:
        L = nq*[log(q)] + n1*[0]
        return (nq, nq, L)


    slope = -2 * log(delta_BKZ(b))
    lq = log(q)
    B = int(floor(log(q) / - slope))    # Number of vectors in the sloppy region
    L = nq*[log(q)] + [lq + i * slope for i in range(1, B+1)] + n1*[0]

    x = 0
    lv = sum (L[:d])
    glv = nq*lq                     # Goal log volume

    while lv > glv:                 # While the current volume exceeeds goal volume, slide the window to the right
        lv -= L[x]
        lv += L[x+d]
        x += 1

    assert x <= B                   # Sanity check that we have not gone too far

    L = L[x:x+d]
    a = max(0, nq - x)             # The length of the [q, ... q] sequence
    B = min(B, d - a)              # The length of the GSA sequence

    diff = glv - lv
    assert abs(diff) < lq               # Sanity check the volume, up to the discretness of index error
    for i in range(a, a+B):        # Small shift of the GSA sequence to equiliBrate volume
        L[i] += diff / B
    lv = sum(L)
    assert abs(lv/glv - 1) < 1e-6        # Sanity check the volume

    return (a, a + B, L)


def construct_BKZ_shape_randomized(q, nq, n1, b):
    """ Simulate the (log) shape of a basis after the reduction of
        a [q ... q, 1 ... 1] shape after a randomization and a BKZ-b reduction
        (such that no GS vectors gets smaller than 1)
    """
    glv = nq * log(q)
    d = nq+n1
    L = []

    slope = -2 * log(delta_BKZ(b))
    li = 0
    lv = 0
    for i in range(d):
        li -= slope
        lv += li
        if lv>glv:
            break
        L = [li]+L
    B = len(L)                  # The length of the sloppy sequence
    L += (d-B)*[0]
    a = 0                        # The length of the [q, ... q] sequence

    lv = sum(L)
    diff = lv - glv
    #print diff, li
    #assert abs(diff) < li          # Sanity check the volume, up to the discretness of index error
    for i in range(a, a+B):        # Small shift of the GSA sequence to equiliBrate volume
        L[i] -= diff / B
    lv = sum(L)
    assert abs(lv/glv - 1) < 1e-6        # Sanity check the volume

    return (a, a + B, L)


def BKZ_first_length(q, nq, n1, b):
    """ Simulate the length of the shortest expected vector in the first b-block
        after randomization (killong q-vectors) and a BKZ-b reduction.
    """

    (_, _, L) = construct_BKZ_shape_randomized(q, nq, n1, b)
    l = exp(L[0])                # Compute the root-volume of the first block
    return l


def BKZ_last_block_length(q, nq, n1, b):
    """ Simulate the length of the expected Gram-Schmidt vector at position d-b (d = n+m)
        after a BKZ-b reduction.
    """

    (_, _, L) = construct_BKZ_shape(q, nq, n1, b)
    return exp(L[nq + n1 - b])

