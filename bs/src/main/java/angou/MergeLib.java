package angou;

import android.content.Context;
import java.io.*;

public class MergeLib {
    public static void merge(Context context) throws IOException {
        // Nama file hasil gabungan → tambahkan .so
        File outFile = new File(context.getFilesDir(), "query.so");

        // Gabungkan semua part
        String[] parts = {"xa","xb","xc","xd","xe","xf","xg","xh","xi","xj","xk","xl","xm","xn","xo"};
        try (FileOutputStream fos = new FileOutputStream(outFile)) {
            byte[] buffer = new byte[8192];
            for (String part : parts) {
                try (InputStream is = context.getAssets().open(part)) {
                    int len;
                    while ((len = is.read(buffer)) != -1) {
                        fos.write(buffer, 0, len);
                    }
                }
            }
        }

        // Setelah selesai gabung → load library
        System.load(outFile.getAbsolutePath());
    }
}
