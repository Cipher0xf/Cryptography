class EC_EIGamal:

    def __init__(self, a, b, p):
        self.a = a
        self.b = b
        self.p = p
        self.sk = 0
        self.P = [0, 0]
        self.pk = [0, 0]
        self.plaintext = [0, 0, 0]
        self.ciphertext = [0, 0, 0]

    def key_generation(self, sk, Px, Py):
        self.sk = sk
        self.P = [Px, Py]
        pk = ec_nP(sk, Px, Py, self.a, self.b, self.p)
        self.pk = pk

    def encrypt(self, m, r):
        temp1 = ec_nP(r, self.P[0], self.P[1], self.a, self.b, self.p)
        temp2 = ec_nP(r, self.pk[0], self.pk[1], self.a, self.b, self.p)
        c = (m * temp2[0]) % self.p
        return [temp1[0], temp1[1], c]

    def decrypt(self, x, y, c):
        temp = ec_nP(self.sk, x, y, self.a, self.b, self.p)
        m = (c * inverse(temp[0], self.p)) % self.p
        return [temp[0], temp[1], m]

    def elliptic_curve_points(self):
        print("EC points:")
        for x in range(self.p):
            for y in range(self.p):
                if pow(y, 2, self.p) == (pow(x, 3, self.p) + self.a * x + self.b) % self.p:
                    print("(", x, ",", y, ")")

    def elliptic_curve_public_key(self, sk, Px, Py):
        print("public key:")
        self.key_generation(sk, Px, Py)
        print("E ( F", self.p, "): y ^ 2 = x ^ 3 + ", self.a, "x +", self.b)
        print("P =", self.P)
        print("Q =", self.pk)

    def elliptic_curve_ciphertext(self, m, r):
        print("ciphertext:")
        ciphertext = self.encrypt(m, r)
        self.ciphertext = ciphertext
        print("(x1,y1,c) =", ciphertext)

    def elliptic_curve_plaintext(self):
        print("plaintext:")
        x = self.ciphertext[0]
        y = self.ciphertext[1]
        c = self.ciphertext[2]
        plaintext = self.decrypt(x, y, c)
        self.plaintext = plaintext
        print("(x',y',m') =", plaintext)


# 求逆元
def inverse(x, p):
    for i in range(1, p):
        if (i * x) % p == 1:
            return i
    return -1


# 求最大公约数
def gcd(a, b):
    if b == 0:
        return a
    else:
        return gcd(b, a % b)


# 椭圆曲线域上加法P+Q
def ec_add(x1, y1, x2, y2, a, b, p):
    flag = 1

    # P = Q
    if x1 == x2 and y1 == y2:
        member = 3 * (x1 ** 2) + a
        denominator = 2 * y1
    # P ≠ Q
    else:
        member = y2 - y1
        denominator = x2 - x1
        if member * denominator < 0:
            flag = 0
            member = abs(member)
            denominator = abs(denominator)

    # 分子分母约分
    gcd_m_d = gcd(member, denominator)
    member = member // gcd_m_d
    denominator = denominator // gcd_m_d

    # 求分母逆元
    inv_d = inverse(denominator, p)
    k = member * inv_d
    if flag == 0:
        k = -k
    k = k % p

    x3 = (k ** 2 - x1 - x2) % p
    y3 = (k * (x1 - x3) - y1) % p
    return [x3, y3]


# 倍运算nP
def ec_nP(n, Px, Py, a, b, p):
    temp_x = Px
    temp_y = Py
    for i in range(n - 1):
        nP = ec_add(temp_x, temp_y, Px, Py, a, b, p)
        temp_x = nP[0]
        temp_y = nP[1]
    return [temp_x, temp_y]


# ECE = EC_EIGamal(1, 6, 11)
# ECE = EC_EIGamal(2, 3, 17)
print(ec_add(2, 7, 11, 8, 2, 3, 17))
print(ec_nP(2, 2, 7, 2, 3, 17))
print(ec_nP(4, 2, 7, 2, 3, 17))
# ECE.elliptic_curve_points()
# ECE.elliptic_curve_public_key(5, 2, 7)
# ECE.elliptic_curve_ciphertext(3, 7)
# ECE.elliptic_curve_plaintext()
