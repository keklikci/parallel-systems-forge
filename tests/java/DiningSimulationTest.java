public final class DiningSimulationTest {
    private DiningSimulationTest() {}

    public static void main(String[] args) throws InterruptedException {
        completesConfiguredRounds();
        acceptsZeroRounds();
        rejectsInvalidConfiguration();
        System.out.println("DiningSimulationTest: all tests passed");
    }

    private static void completesConfiguredRounds() throws InterruptedException {
        DiningSimulation.Result result = DiningSimulation.run(5, 20, 42L);
        require(result.meals() == 100, "every philosopher should complete every round");
        require(result.maximumConcurrentMeals() >= 1, "at least one meal should occur");
        require(result.maximumConcurrentMeals() <= 2, "adjacent philosophers must not eat together");
    }

    private static void acceptsZeroRounds() throws InterruptedException {
        DiningSimulation.Result result = DiningSimulation.run(3, 0, 7L);
        require(result.meals() == 0, "zero rounds should produce zero meals");
    }

    private static void rejectsInvalidConfiguration() throws InterruptedException {
        boolean rejected = false;
        try {
            DiningSimulation.run(1, 1, 1L);
        } catch (IllegalArgumentException expected) {
            rejected = true;
        }
        require(rejected, "fewer than two philosophers should be rejected");
    }

    private static void require(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
    }
}
