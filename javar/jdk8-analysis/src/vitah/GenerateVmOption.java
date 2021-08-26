package vitah;

import java.util.Arrays;

/**
 * 生成vm参数，启动时把输出内容附加到vm参数中才能实现jdk的打印
 * @author vitah
 */
public class GenerateVmOption {

    public static void main(String[] args) {
        String pathTotal = System.getProperty("sun.boot.class.path");
        String[] paths = pathTotal.split(":");
        Arrays.stream(paths).forEach(System.out::println);

        String vmOption = "-Dsun.boot.class.path="
            + "/Users/vitah/Downloads/dev/vitah/kennen/javar/jdk8-analysis/out/production/jdk8-analysis"
            + ":"
            + pathTotal;
        System.out.println(vmOption);
    }
}
