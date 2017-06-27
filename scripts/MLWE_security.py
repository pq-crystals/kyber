from math import *
from model_BKZ import *

log_infinity = 9999

STEPS_b = 5
STEPS_m = 5


class MLWEParameterSet:
    def __init__(self, n, d, m, k, q, distr="binomial"):
        self.n = n          # Ring Dimension
        self.d = d          # MLWE Dimension (over the ring)
        self.m = m          # Number of Ring-Samples
        self.k = k          # Error Parameter
        self.q = q          # Modulus
        self.distr = distr  # Type of distribution : binomial or uniform (parameter k)


def LWE_primal_cost(q, n, m, s, b, cost_svp=svp_classical, verbose=False):
    """ Return the cost of the primal attack using m samples and blocksize b (infinity = fail)
    """
    d = n + m
    delta = delta_BKZ(b)
    if verbose:
        print("Primal attacks uses block-size %d and %d samples"%(b, m))
    
    if s * sqrt(b) < BKZ_last_block_length(q, m, n, b):
        return cost_svp(b)
    else:
        return log_infinity


def LWE_dual_cost(q, n, m, s, b, cost_svp=svp_classical, verbose=False):
    """ Return the cost of the dual attack using m samples and blocksize b (infinity = fail)
    """
    d = n + m
    l = BKZ_first_length(q, n, m, b)
    tau = l * s / q
    log2_eps = - 2 * pi * pi * tau**2 / log(2)
    log2_R = max(0, - 2 * log2_eps - nvec_sieve(b))
    if verbose:
        print ("Dual attacks uses block-size %d and %d samples"%(b, m))
        print ("shortest vector used has length l=%.2f, q=%d, `l<q'= %d"%(l, q, l<q))
        print ("log2(epsilon) = %.2f, log2 nvector per run %.2f"%(log2_eps, nvec_sieve(b)))
    return cost_svp(b) + log2_R


def MLWE_optimize_attack(q, n, max_m, s, cost_attack=LWE_primal_cost, cost_svp=svp_classical, verbose=True):
    """ Find optimal parameters for a given attack
    """
    best_cost = log_infinity
    for b in range(50, n+max_m, STEPS_b):
        if cost_svp(b) > best_cost:
            break
        for m in range(max(5, b-n), max_m, STEPS_m):
            cost = cost_attack(q, n, m, s, b, cost_svp)
            if cost<best_cost:
                best_cost = cost
                best_m = m
                best_b = b

    cost_attack(q, n, best_m, s, best_b, cost_svp=svp_classical, verbose=verbose)
    return (best_m, best_b, best_cost)


def check_eq(m_pc, m_pq, m_pp):
    if (m_pc != m_pq):
        print("m and b not equals among the three models")
    if (m_pq != m_pp):
        print("m and b not equals among the three models")


def MLWE_summarize_attacks(ps):
    """ Create a report on the best primal and dual BKZ attacks on an MLWE instance
    """
    q = ps.q
    n = ps.n * ps.d
    max_m = ps.n * ps.m
    
    if ps.distr=="binomial":
        s = sqrt(ps.k /2.)
    elif ps.distr=="uniform":
        k = ps.k
        s = sqrt(sum([i**2 for i in range(-k, k+1)])/(2*k+1))
    else:
        raise ValueError("Unknown distribution "+ps.distr)
    
    (m_pc, b_pc, c_pc) = MLWE_optimize_attack(q, n, max_m, s, cost_attack=LWE_primal_cost, cost_svp=svp_classical, verbose=True)
    (m_pq, b_pq, c_pq) = MLWE_optimize_attack(q, n, max_m, s, cost_attack=LWE_primal_cost, cost_svp=svp_quantum, verbose=False)
    (m_pp, b_pp, c_pp) = MLWE_optimize_attack(q, n, max_m, s, cost_attack=LWE_primal_cost, cost_svp=svp_plausible, verbose=False)

    check_eq(m_pc, m_pq, m_pp)
    check_eq(b_pc, b_pq, b_pp)

    print("Primal & %d & %d & %d & %d & %d"%(m_pq, b_pq, int(floor(c_pc)), int(floor(c_pq)), int(floor(c_pp))))

    (m_pc, b_pc, c_pc) = MLWE_optimize_attack(q, n, max_m, s, cost_attack=LWE_dual_cost, cost_svp=svp_classical, verbose=True)
    (m_pq, b_pq, c_pq) = MLWE_optimize_attack(q, n, max_m, s, cost_attack=LWE_dual_cost, cost_svp=svp_quantum, verbose=False)
    (m_pp, b_pp, c_pp) = MLWE_optimize_attack(q, n, max_m, s, cost_attack=LWE_dual_cost, cost_svp=svp_plausible, verbose=False)

    check_eq(m_pc, m_pq, m_pp)
    check_eq(b_pc, b_pq, b_pp)

    print("Dual & %d & %d & %d & %d & %d "%(m_pq, b_pq, int(floor(c_pc)), int(floor(c_pq)), int(floor(c_pp))))
    return (b_pq, int(floor(c_pc)), int(floor(c_pq)), int(floor(c_pp)))
