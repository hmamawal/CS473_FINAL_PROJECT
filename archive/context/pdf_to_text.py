import argparse
from PyPDF2 import PdfReader

def pdf_to_text(input_pdf: str, output_txt: str):
    """
    Converts a PDF file to a text file.
    
    Usage:
        pdf_to_text("input.pdf", "output.txt")
    
    Parameters:
        input_pdf (str): Path to the input PDF file.
        output_txt (str): Path to the output text file.
    """
    reader = PdfReader(input_pdf)
    with open(output_txt, "w", encoding="utf-8") as txt_file:
        for page in reader.pages:
            txt_file.write(page.extract_text())
            txt_file.write("\n")  # Add a newline between pages

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Convert a PDF file to a text file.")
    parser.add_argument("input_pdf", help="Path to the input PDF file")
    parser.add_argument("output_txt", help="Path to the output text file")
    args = parser.parse_args()
    
    pdf_to_text(args.input_pdf, args.output_txt)
    print(f"Text extracted to {args.output_txt}")