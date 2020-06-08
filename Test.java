public class Test

{
    public static void test(){
        String modelPath = System.getProperty("MODEL_PATH");
        long sp_ptr = sentencepiece_interfaceJNI.get_model(modelPath);
        String pices = sentencepiece_interfaceJNI.encode_pieces(sp_ptr, "unaffable");
        String ids = sentencepiece_interfaceJNI.encode_ids(sp_ptr, "unaffable");
        String orig_pieces = "▁una ff able ";
        String orig_ids =  "219 4901 2885 ";
        assert pices.equals(orig_pieces): (orig_pieces + " != " + pices);
        assert ids.equals(orig_ids): (orig_ids + " != " + ids);

        sentencepiece_interfaceJNI.unload_model(sp_ptr);

    }
    public static void main(String argv[]) 
    {
        System.loadLibrary("sentencepiece_interface");
        test();
    }
}
