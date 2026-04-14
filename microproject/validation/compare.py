import numpy as np
import matplotlib.pyplot as plt
from scipy.special import erf
from scipy.optimize import brentq

k_liquid  = 0.6       # Вт/(м·К)
c_liquid  = 4180.0    # Дж/(кг·К)
rho       = 1000.0    # кг/м³
L         = 334000.0  # Дж/кг
T_melt    = 0.0       # °C
T_hot     = 50.0      # °C (из stefan_1d.hpp)

alpha = k_liquid / (rho * c_liquid)
St = c_liquid * (T_hot - T_melt) / L          # число Стефана
print(f"St = {St:.4f}")
print(f"alpha = {alpha:.4e} м²/с")


def stefan_equation(lam):
    return lam * np.sqrt(np.pi) * np.exp(lam**2) * erf(lam) - St

lam = brentq(stefan_equation, 0.001, 5.0)
print(f"lambda = {lam:.6f}")


def front_analytical(t):
    return 2 * lam * np.sqrt(alpha * t)

try:
    data = np.loadtxt("results/front.csv", delimiter=",", skiprows=1)
    t_num = data[:, 0]
    x_num = data[:, 1]
    has_numerical = True
    print(f"Загружено {len(t_num)} точек из results/front.csv")
except Exception as e:
    print(f"Не удалось загрузить results/front.csv: {e}")
    has_numerical = False

fig, ax = plt.subplots(1, 1, figsize=(10, 6))

if has_numerical:
    t_max = t_num[-1]
    t_ana = np.linspace(1e-8, t_max, 500)
    x_ana = front_analytical(t_ana)

    ax.plot(t_num, x_num * 100, 'b.', markersize=3, label='Численное решение')
    ax.plot(t_ana, x_ana * 100, 'r-', linewidth=2, label='Аналитика: $s = 2\\lambda\\sqrt{\\alpha t}$')
else:
    t_ana = np.linspace(1e-8, 0.01, 500)
    x_ana = front_analytical(t_ana)
    ax.plot(t_ana, x_ana * 100, 'r-', linewidth=2, label='Аналитика: $s = 2\\lambda\\sqrt{\\alpha t}$')

ax.set_xlabel('Время, с', fontsize=14)
ax.set_ylabel('Положение фронта, см', fontsize=14)
ax.set_title('Задача Стефана: 1d', fontsize=16)
ax.legend(fontsize=12)
ax.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('results/validation.png', dpi=150)
plt.show()
print("done")
