#include <iostream>
#include <limits>
#include <cmath>

class IStatistics {
public:
	virtual ~IStatistics() {}

	virtual void update(double next) = 0;
	virtual double eval() const = 0;
	virtual const char * name() const = 0;
};

class Min : public IStatistics {
public:
	Min() : m_min{std::numeric_limits<double>::max()} {
	}

	void update(double next) override {
		if (next < m_min) {
			m_min = next;
		}
	}

	double eval() const override {
		return m_min;
	}

	const char * name() const override {
		return "min";
	}

private:
	double m_min;
};

class Max : public IStatistics {
public:
	Max() : m_max{std::numeric_limits<double>::lowest()} {
	}

	void update(double next) override {
		if (next > m_max) {
			m_max = next;
		}
	}

	double eval() const override {
		return m_max;
	}

	const char * name() const override {
		return "max";
	}

private:
	double m_max;
};

class Mean : public IStatistics {
public:
	Mean() : m_mean{0.0}, m_count{0} {
	}

	// Используем алгоритм Уэлфорда https://habr.com/ru/articles/333426/
	void update(double next) override {
		m_count++;
		m_mean = m_mean + ((next - m_mean) / m_count);
	}

	double eval() const override {
		return m_mean;
	}

	const char * name() const override {
		return "mean";
	}

private:
	double m_mean;
	size_t m_count;
};

// Для расчёта std воспользуемся тем же алгоритмом Уэлфорда https://habr.com/ru/articles/333426/
// https://stackoverflow.com/questions/15638612/calculating-mean-and-standard-deviation-of-the-data-which-does-not-fit-in-memory/15638726#15638726
class Std : public IStatistics {
public:
	Std() : m_m2{0.0}, m_count{0} {
	}

	void update(double next) override {
        m_count++;

		double old_mean = m_mean.eval();
        m_mean.update(next);

		double new_mean = m_mean.eval();
        m_m2 = m_m2 + ((next - old_mean) * (next - new_mean));
	}

	double eval() const override {
		// В задании не указано, что мы оцениваем: выборку или генеральную совокупность
		// Исходя из того, что говорится о всей последовательности и в тексте задания слово
		// "выборка" упоминается, будем считать, что имеется в виду СКО для генеральной совокупности
		return std::sqrt(m_m2 / (m_count));
		// Если вдруг понадобится СКО для выборки, то используем следующий код
		// if (m_count < 2) {
		//    // Если у нас один элемент, то его дисперсия (и СКО) равна нулю
		//    return 0.0;
		// }
		// return std::sqrt(m_m2 / (m_count - 1));
	}

	const char * name() const override {
		return "std";
	}

private:
	double m_m2;
	size_t m_count;
	// Используем композицию для среднего
	// гипотетически здесь могло быть приватное наследование
	Mean m_mean;
};

int main() {

	const size_t statistics_count = 4;
	IStatistics *statistics[statistics_count];

	statistics[0] = new Min{};
	statistics[1] = new Mean{};
	statistics[2] = new Std{};
	statistics[3] = new Max{};

	double val = 0;
	// // для тестов
	// for (val = 0; val < 11; val++) {
	// 	std::cout << val << " ";
	// }
    // std::cout << std::endl;
	// for (val = 0; val < 11; val++) {
	while (std::cin >> val) {
		for (size_t i = 0; i < statistics_count; ++i) {
			statistics[i]->update(val);
		}
	}

	// Handle invalid input data
	if (!std::cin.eof() && !std::cin.good()) {
		std::cerr << "Invalid input data\n";
		return 1;
	}

	// Print results if any
	for (size_t i = 0; i < statistics_count; ++i) {
		std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
	}

	// Clear memory - delete all objects created by new
	for (size_t i = 0; i < statistics_count; ++i) {
		delete statistics[i];
	}

	return 0;
}