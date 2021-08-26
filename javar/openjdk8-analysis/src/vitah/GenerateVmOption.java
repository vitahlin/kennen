package vitah;

import java.util.Arrays;

public class GenerateVmOption {
    public static void main(String[] args) {
        String pathTotal = System.getProperty("sun.boot.class.path");
        String[] paths = pathTotal.split(":");
        Arrays.stream(paths).forEach(System.out::println);

        String vmOption = "-Dsun.boot.class.path="
            + "/Users/vitah/Downloads/dev/vitah/kennen/javar/openjdk8-analysis/out/production/openjdk8-analysis"
            + ":"
            + pathTotal;
        System.out.println(vmOption);
    }
}
