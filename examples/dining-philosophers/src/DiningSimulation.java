import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/** A finite, deterministic dining-philosophers simulation without UI dependencies. */
public final class DiningSimulation {
    public record Result(int philosophers, int rounds, long seed, int meals, int maximumConcurrentMeals) {}

    private DiningSimulation() {}

    public static Result run(int philosopherCount, int rounds, long seed) throws InterruptedException {
        if (philosopherCount < 2 || rounds < 0) {
            throw new IllegalArgumentException("philosopherCount must be >= 2 and rounds must be >= 0");
        }
        Table table = new Table(philosopherCount);
        List<Thread> philosophers = new ArrayList<>();
        for (int id = 0; id < philosopherCount; ++id) {
            final int philosopherId = id;
            philosophers.add(new Thread(() -> dine(table, philosopherId, rounds, seed), "philosopher-" + id));
        }
        philosophers.forEach(Thread::start);
        for (Thread philosopher : philosophers) philosopher.join();
        return new Result(philosopherCount, rounds, seed, table.meals, table.maximumConcurrentMeals);
    }

    private static void dine(Table table, int id, int rounds, long seed) {
        Random random = new Random(seed + id * 31L);
        try {
            for (int round = 0; round < rounds; ++round) {
                Thread.sleep(random.nextInt(4));
                table.takeForks(id);
                try {
                    Thread.sleep(random.nextInt(4));
                } finally {
                    table.putForks(id);
                }
            }
        } catch (InterruptedException interrupted) {
            Thread.currentThread().interrupt();
        }
    }

    private static final class Table {
        private final boolean[] eating;
        private int meals;
        private int concurrentMeals;
        private int maximumConcurrentMeals;

        private Table(int philosopherCount) { eating = new boolean[philosopherCount]; }

        private synchronized void takeForks(int id) throws InterruptedException {
            int left = (id + eating.length - 1) % eating.length;
            int right = (id + 1) % eating.length;
            while (eating[left] || eating[right]) wait();
            eating[id] = true;
            ++concurrentMeals;
            ++meals;
            maximumConcurrentMeals = Math.max(maximumConcurrentMeals, concurrentMeals);
        }

        private synchronized void putForks(int id) {
            if (!eating[id]) throw new IllegalStateException("philosopher released forks it did not hold");
            eating[id] = false;
            --concurrentMeals;
            notifyAll();
        }
    }
}
