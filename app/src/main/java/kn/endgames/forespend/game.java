package kn.endgames.forespend;

import android.app.Activity;
import android.os.Bundle;
import android.content.res.AssetManager;
import java.io.*;

public class game extends Activity {

    static {
        System.loadLibrary("a13");  // loads liba13.so
    }

    // call your native init(basePath)
    public static native void nativeInit(String basePath);

    @Override
    protected void onCreate(Bundle saved) {
        super.onCreate(saved);

        // get path: /data/data/kn.endgames.forespend/files/
        String base = getFilesDir().getAbsolutePath();
        nativeInit(base);

        copyAssetDir("res", base + "/res");

        // after copying, launch the NativeActivity or
        // directly start your native code (depends on your setup)
    }

    private void copyAssetDir(String assetDir, String destDir) {
        AssetManager am = getAssets();
        try {
            String[] assets = am.list(assetDir);
            if (assets == null || assets.length == 0) {
                // single file
                copyAsset(assetDir, destDir);
                return;
            }

            File outDir = new File(destDir);
            if (!outDir.exists()) outDir.mkdirs();

            for (String file : assets) {
                copyAssetDir(assetDir + "/" + file, destDir + "/" + file);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void copyAsset(String assetPath, String destPath) {
        try {
            File outFile = new File(destPath);
            outFile.getParentFile().mkdirs();

            InputStream in = getAssets().open(assetPath);
            OutputStream out = new FileOutputStream(outFile);

            byte[] buf = new byte[4096];
            int read;
            while ((read = in.read(buf)) > 0) {
                out.write(buf, 0, read);
            }

            in.close();
            out.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
