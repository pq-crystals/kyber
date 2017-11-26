from math import log
from Kyber_failure import p2_cyclotomic_error_probability
from MLWE_security import MLWE_summarize_attacks, MLWEParameterSet


class KyberParameterSet:
    def __init__(self, n, m, ks, ke,  q, rqk, rqc, rq2):
        self.n = n
        self.m = m
        self.ks= ks     # binary distribution for the secret key
        self.ke = ke    # binary distribution for the ciphertext errors
        self.q = q 
        self.rqk = rqk  # 2^(bits in the public key)  
        self.rqc = rqc  # 2^(bits in the first ciphertext)
        self.rq2 = rq2  # 2^(bits in the second ciphertext)


def Kyber_to_MLWE(kps):
    if kps.ks != kps.ke:
        raise "The security script does not handle different error parameter in secrets and errors (ks != ke) "

    return MLWEParameterSet(kps.n, kps.m, kps.m + 1, kps.ks, kps.q)


def communication_costs(ps):
    """ Compute the communication cost of a parameter set
    :param ps: Parameter set (ParameterSet)
    :returns: (cost_Alice, cost_Bob) (in Bytes)
    """    
    A_space = 256 + ps.n * ps.m * log(ps.rqk)/log(2)
    B_space = ps.n * ps.m * log(ps.rqc)/log(2) + ps.n * log(ps.rq2)/log(2) + 256
    return (int(round(A_space))/8., int(round(B_space))/8.)


def summarize(ps):
    print ("params: ", ps.__dict__)
    print ("com costs: ", communication_costs(ps))
    F, f = p2_cyclotomic_error_probability(ps)
    print ("failure: %.1f = 2^%.1f"%(f, log(f + 2.**(-300))/log(2)))


if __name__ == "__main__":
    # Parameter sets
    ps_recommended = KyberParameterSet(256, 3, 4, 4, 7681, 2**11, 2**11, 2**3)
    ps_light = KyberParameterSet(256, 2, 5, 5, 7681, 2**11, 2**11, 2**3)
    ps_paranoid = KyberParameterSet(256, 4, 3, 3, 7681, 2**11, 2**11, 2**3)

    # Analyses
    print ("Kyber512 (light):")
    print ("--------------------")
    print ("security:")
    MLWE_summarize_attacks(Kyber_to_MLWE(ps_light))
    summarize(ps_light)
    print ()

    print ("Kyber768 (recommended):")
    print ("--------------------")
    print ("security:")
    MLWE_summarize_attacks(Kyber_to_MLWE(ps_recommended))
    summarize(ps_recommended)
    print ()

    print ("Kyber1024 (paranoid):")
    print ("--------------------")
    print ("security:")
    MLWE_summarize_attacks(Kyber_to_MLWE(ps_paranoid))
    summarize(ps_paranoid)
    print ()
