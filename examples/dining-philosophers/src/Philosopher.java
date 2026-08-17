import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.net.URL;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

/** Swing launcher for the dependency-free DiningSimulation core. */
public final class Philosopher {
    private Philosopher() {}

    public static void main(String[] args) {
        int rounds = args.length > 0 ? Integer.parseInt(args[0]) : 5;
        long seed = args.length > 1 ? Long.parseLong(args[1]) : 1L;
        SwingUtilities.invokeLater(() -> createWindow(rounds, seed));
    }

    private static void createWindow(int rounds, long seed) {
        JFrame frame = new JFrame("Dining Philosophers");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new BorderLayout(12, 12));
        frame.add(new JLabel(loadIcon("/schemas/spaghetti_yellow.jpg")), BorderLayout.CENTER);
        JPanel controls = new JPanel(new GridLayout(2, 1, 4, 4));
        JLabel result = new JLabel("Ready: " + rounds + " rounds, seed " + seed);
        JButton start = new JButton("Start simulation");
        start.addActionListener(event -> {
            start.setEnabled(false);
            result.setText("Running...");
            Thread worker = new Thread(() -> {
                try {
                    DiningSimulation.Result simulation = DiningSimulation.run(5, rounds, seed);
                    SwingUtilities.invokeLater(() -> result.setText(
                        "Meals: " + simulation.meals() + ", maximum concurrent: "
                            + simulation.maximumConcurrentMeals()));
                } catch (InterruptedException interrupted) {
                    Thread.currentThread().interrupt();
                    SwingUtilities.invokeLater(() -> result.setText("Simulation interrupted"));
                } finally {
                    SwingUtilities.invokeLater(() -> start.setEnabled(true));
                }
            }, "dining-simulation");
            worker.start();
        });
        controls.add(result);
        controls.add(start);
        frame.add(controls, BorderLayout.SOUTH);
        frame.setMinimumSize(new Dimension(420, 260));
        frame.pack();
        frame.setLocationByPlatform(true);
        frame.setVisible(true);
    }

    private static ImageIcon loadIcon(String resource) {
        URL location = Philosopher.class.getResource(resource);
        if (location == null) throw new IllegalStateException("Missing GUI resource: " + resource);
        return new ImageIcon(location);
    }
}
