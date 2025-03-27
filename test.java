import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class JavaVulnerabilityChecker {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            CodeAnalyzerUI ui = new CodeAnalyzerUI();
            ui.createAndShowGUI();
        });
    }
}

class CodeAnalyzerUI {
    private JFrame frame;
    private JTextArea resultArea;
    private JProgressBar progressBar;
    private File selectedFile;

    public void createAndShowGUI() {
        frame = new JFrame("Java Vulnerability Checker");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(600, 400);

        JPanel panel = new JPanel();
        panel.setLayout(new FlowLayout());

        JButton uploadButton = new JButton("Upload .txt File");
        JButton submitButton = new JButton("Submit");
        resultArea = new JTextArea(15, 50);
        resultArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(resultArea);
        progressBar = new JProgressBar(0, 100);
        progressBar.setStringPainted(true);

        uploadButton.addActionListener(e -> chooseFile());
        submitButton.addActionListener(e -> analyzeFile());

        panel.add(uploadButton);
        panel.add(submitButton);
        frame.add(panel, BorderLayout.NORTH);
        frame.add(scrollPane, BorderLayout.CENTER);
        frame.add(progressBar, BorderLayout.SOUTH);

        frame.setVisible(true);
    }

    private void chooseFile() {
        JFileChooser fileChooser = new JFileChooser();
        int returnValue = fileChooser.showOpenDialog(frame);
        if (returnValue == JFileChooser.APPROVE_OPTION) {
            selectedFile = fileChooser.getSelectedFile();
            resultArea.setText("File selected: " + selectedFile.getName() + "\n");
        }
    }

    private void analyzeFile() {
        if (selectedFile == null) {
            resultArea.setText("No file selected!\n");
            return;
        }
        new Thread(() -> {
            try {
                List<String> codeLines = readFile(selectedFile);
                List<String> vulnerabilities = CodeAnalyzer.checkForVulnerabilities(codeLines, progressBar);
                
                SwingUtilities.invokeLater(() -> {
                    if (vulnerabilities.isEmpty()) {
                        resultArea.append("No vulnerabilities found.\n");
                    } else {
                        resultArea.append("Potential vulnerabilities found:\n");
                        for (String vulnerability : vulnerabilities) {
                            resultArea.append(vulnerability + "\n");
                        }
                    }
                    progressBar.setValue(100);
                });
            } catch (IOException e) {
                SwingUtilities.invokeLater(() -> resultArea.setText("Error reading file.\n"));
            }
        }).start();
    }

    private List<String> readFile(File file) throws IOException {
        List<String> lines = new ArrayList<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            String line;
            while ((line = reader.readLine()) != null) {
                lines.add(line);
            }
        }
        return lines;
    }
}

class CodeAnalyzer {
    public static List<String> checkForVulnerabilities(List<String> codeLines, JProgressBar progressBar) {
        List<String> vulnerabilities = new ArrayList<>();
        int lineNumber = 0;
        int totalLines = codeLines.size();

        for (String line : codeLines) {
            lineNumber++;
            if (line.contains("eval(")) {
                vulnerabilities.add("Line " + lineNumber + ": Use of eval() is a security risk.");
            }
            if (line.contains(".getDeclaredMethod(") || line.contains(".getMethod(") || line.contains(".invoke(")) {
                vulnerabilities.add("Line " + lineNumber + ": Reflection usage detected, potential security risk.");
            }
            if (line.matches(".*password.*=.*\".*\".*") || line.matches(".*username.*=.*\".*\".*")) {
                vulnerabilities.add("Line " + lineNumber + ": Hardcoded credentials detected.");
            }
            if (line.matches(".*new\\s+int\\[\\d{9,}].*")) {
                vulnerabilities.add("Line " + lineNumber + ": Potential excessive memory allocation detected.");
            }
            if (line.contains("Statement stmt = conn.createStatement()")) {
                vulnerabilities.add("Line " + lineNumber + ": SQL Injection risk detected. Use PreparedStatement instead.");
            }
            if (line.contains("new FileReader(") || line.contains("new BufferedReader(new FileReader(")) {
                vulnerabilities.add("Line " + lineNumber + ": Arbitrary file read vulnerability detected.");
            }
            if (line.contains("Class.forName(") || line.contains("loadedClass.getDeclaredConstructor().newInstance()")) {
                vulnerabilities.add("Line " + lineNumber + ": Unsafe reflection detected.");
            }
            
            int progress = (int) (((double) lineNumber / totalLines) * 100);
            SwingUtilities.invokeLater(() -> progressBar.setValue(progress));
        }
        return vulnerabilities;
    }
}